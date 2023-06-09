/*
 **********************************************************************************************************************
 *
 * This file is part of the rfid-controller project.
 *
 * Copyright (c) 2023 Ivan Odinets <i_odinets@protonmail.com>
 *
 * rfid-controller is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * rfid-controller is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with rfid-controller. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "SerialDeviceManager.h"

#include <QDebug>

SerialDeviceManager::SerialDeviceManager(QObject *parent)
    : QObject{parent}
{
    connect(&m_serialDeviceWatcher,&SerialDeviceWatcher::deviceWasAttached,this,&SerialDeviceManager::_handleAttachedSerialDevice);
    connect(&m_serialDeviceWatcher,&SerialDeviceWatcher::deviceWasDetached,this,&SerialDeviceManager::_handleDetachedSerialDevice);
}

void SerialDeviceManager::start()
{
    //Automatically connect all devices, which are matching autoconnect criteria
    if (serialDeviceManagerSettings->serialDeviceAutoconnection()) {   //If autoconnect enabled

        //Go through attached device list and connect enything which is fitting autoconnect rules
        for (const QSerialPortInfo& portInfo : m_serialDeviceWatcher.availableDevices()) {
            if (!serialDeviceManagerSettings->serialDeviceFilter().isMatching(portInfo)) {
                continue;
            }

            if (_tryOpeningSerialDevice(portInfo)) {
                emit serialDeviceWasOpened(portInfo);
            }
        }
    }

    m_serialDeviceWatcher.start();
}

QList<QSerialPortInfo> SerialDeviceManager::openedSerialDevices() const
{
    QList<QSerialPortInfo> result;
    for(SerialDevice* device : m_openedSerialDevices)
        result.append(device->portInfo());

    return result;
}

void SerialDeviceManager::setSerialDeviceAutoconnection(bool state)
{
    serialDeviceManagerSettings->setSerialDeviceAutoconnection(state);
}

void SerialDeviceManager::setSerialDeviceFilter(const SerialPortFilter& newFilter)
{
    serialDeviceManagerSettings->saveSerialDeviceFilter(newFilter);
}

void SerialDeviceManager::appendSerialDeviceFilter(const SerialPortFilter& filter)
{
    serialDeviceManagerSettings->appendSerialDeviceFilter(filter);
}

void SerialDeviceManager::setDefaultPortConfig(const SerialPortConfig& config)
{
    serialDeviceManagerSettings->setDefaultSerialPortCongiguration(config);
}

void SerialDeviceManager::serialDeviceOpeningRequested(const QSerialPortInfo& port)
{
    //This method should be invoked only by interactions with the user
    if (_tryOpeningSerialDevice(port)) {    //Try to open the device:
        emit serialDeviceWasOpened(port);   //If success - inform user about it
    } else {                                //Or:
        emit serialDeviceWasClosed(port);   //Inform user about failure in opening
    }
}

void SerialDeviceManager::serialDeviceClosureRequested(const QSerialPortInfo& deviceDetails)
{
    //This method should be invoked only by interactions with the user
    for (int i = 0; i < m_openedSerialDevices.count(); i++) {
        SerialDevice* serialDevice = m_openedSerialDevices.at(i);
        if (serialDevice->portInfo().portName() == deviceDetails.portName()) {
            m_openedSerialDevices.removeOne(serialDevice);
            serialDevice->close();
            serialDevice->deleteLater();
            return;
        }
    }
}

void SerialDeviceManager::_handleAttachedSerialDevice(const QSerialPortInfo& port)
{
    //This method is invoked when new device is connected to the computer
    if (!serialDeviceManagerSettings->serialDeviceAutoconnection()) {  //If autoconnect disabled
        emit serialDeviceWasAttached(port);           //Just inform user about new device
        return;
    }

    if (!serialDeviceManagerSettings->serialDeviceFilter().isMatching(port)) {     //If autoconnect enabled, but not matching criteria
        emit serialDeviceWasAttached(port);                       //Jsut inform user about new device
        return;
    }

    if (!_tryOpeningSerialDevice(port)) {     //If autoconnect enabled, criteria are fulfilled, but opening failed
        emit serialDeviceWasAttached(port);   //Still just inform user about new device
        return;
    }

    emit serialDeviceWasOpened(port);         //Inform user, that device was opened!
}

void SerialDeviceManager::_handleDetachedSerialDevice(const QSerialPortInfo& port)
{
    emit serialDeviceWasDetached(port);      //Inform anybody interested that device was disconnected
}

void SerialDeviceManager::_handleClosedSerialDevice()
{
    SerialDevice* device = qobject_cast<SerialDevice*>(sender());
    Q_ASSERT(device != nullptr);

    m_openedSerialDevices.removeOne(device);

    emit serialDeviceWasClosed(device->portInfo());
    device->deleteLater();
}

void SerialDeviceManager::_serialDeviceErrorOccured(QSerialPort::SerialPortError error)
{
    SerialDevice* device = qobject_cast<SerialDevice*>(sender());
    Q_ASSERT(device != nullptr);

    if (error == QSerialPort::NoError)
        return;

    emit errorMessage(_getSerialErrorMessage(error,device->portInfo()));
    device->close();
}

bool SerialDeviceManager::_tryOpeningSerialDevice(const QSerialPortInfo& portInfo)
{
    SerialDevice* device = new SerialDevice(portInfo);
    device->configureSerialPort(serialDeviceManagerSettings->defaultSerialPortConfiguration());

    if (!device->open(QIODevice::ReadOnly)) {
        qDebug() << "Port "<<portInfo.portName()<< " opening failed. Reason: "<<device->errorString();
        emit errorMessage(_getSerialErrorMessage(device->error(),device->portInfo()));
        device->deleteLater();
        return false;
    }

    connect(device,&SerialDevice::deviceClosed,this,&SerialDeviceManager::_handleClosedSerialDevice);
    connect(device,&SerialDevice::errorOccured,this,&SerialDeviceManager::_serialDeviceErrorOccured);
    connect(device,&SerialDevice::keyFound,this,&SerialDeviceManager::keyFound);
    m_openedSerialDevices.append(device);
    return true;
}

QString SerialDeviceManager::_getSerialErrorMessage(QSerialPort::SerialPortError error,const QSerialPortInfo& portInfo)
{
    switch (error) {
    case QSerialPort::NoError:
        Q_ASSERT(false);
    case QSerialPort::DeviceNotFoundError:
        return tr("Port %1 not found").arg(portInfo.portName());
    case QSerialPort::PermissionError:
        return tr("Check access permissions on port %1").arg(portInfo.portName());
    case QSerialPort::OpenError:
        return tr("Error opening port %1").arg(portInfo.portName());
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 8)
    case QSerialPort::ParityError:
        return tr("Parity error occured for port %1").arg(portInfo.portName());
    case QSerialPort::FramingError:
        return tr("Framing error occured for port %1").arg(portInfo.portName());
    case QSerialPort::BreakConditionError:
        return tr("Break condition error occured for port %1").arg(portInfo.portName());
#endif //Only for Qt < 5.15
    case QSerialPort::WriteError:
        return tr("Write error occured for port %1").arg(portInfo.portName());
    case QSerialPort::ReadError:
        return tr("Read error occured for port %1").arg(portInfo.portName());
    case QSerialPort::ResourceError:
        return tr("Resource error occured for port %1").arg(portInfo.portName());
    case QSerialPort::UnsupportedOperationError:
        return tr("Unsupported operation error occured for port %1").arg(portInfo.portName());
    case QSerialPort::UnknownError:
        return tr("Unknown error with port %1").arg(portInfo.portName());
    case QSerialPort::TimeoutError:
        return tr("Timeout error on port %1").arg(portInfo.portName());
    case QSerialPort::NotOpenError:
        return tr("Port %1 is not opened").arg(portInfo.portName());
    }
    Q_ASSERT(false);
    return QString();
}
