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

#include "InputDeviceManager.h"

InputDeviceManager::InputDeviceManager(QObject *parent)
    : QObject{parent}
{
    connect(&m_inputDeviceWatcher,&InputDeviceWatcher::deviceWasAttached,this,&InputDeviceManager::_handleAttachedInputDevice);
    connect(&m_inputDeviceWatcher,&InputDeviceWatcher::deviceWasDetached,this,&InputDeviceManager::_handleDetachedInputDevice);
}

void InputDeviceManager::start()
{
    //Automatically connect all devices, which are matching autoconnect criteria
    if (inputDeviceManagerSettings->inputDeviceAutoconnection()) {   //If autoconnect enabled

        //Go through attached device list and connect enything which is fitting autoconnect rules
        for (const InputDeviceInfo& deviceInfo : m_inputDeviceWatcher.availableDevices()) {
            if (!inputDeviceManagerSettings->inputDeviceFilter().isMatching(deviceInfo)) {
                continue;
            }

            if (_tryOpeningInputDevice(deviceInfo)) {
                emit inputDeviceWasOpened(deviceInfo);
            }
        }
    }

    m_inputDeviceWatcher.start();
}

InputDeviceInfoList InputDeviceManager::openedInputDevices() const
{
    InputDeviceInfoList result;
    for (InputDevice* device : m_openedInputDevices)
        result.append(device->deviceInfo());

    return result;
}

void InputDeviceManager::setInputDeviceAutoconnection(bool state)
{
    inputDeviceManagerSettings->setInputDeviceAutoconnection(state);
}

void InputDeviceManager::setInputDeviceFilter(const InputDeviceFilter& filter)
{
    inputDeviceManagerSettings->setInputDeviceFilter(filter);
}

void InputDeviceManager::appendInputDeviceFilter(const InputDeviceFilter& filter)
{
    inputDeviceManagerSettings->appendInputDeviceFilter(filter);
}

void InputDeviceManager::inputDeviceOpeningRequested(const InputDeviceInfo& deviceDetails)
{
    //This method should be invoked only by interactions with the user
    if (_tryOpeningInputDevice(deviceDetails)) {  //Try to open the device:
        emit inputDeviceWasOpened(deviceDetails); //    If success - inform user about it
    } else {                                      //Or:
        emit inputDeviceWasClosed(deviceDetails); //    Inform user about failure in opening
    }                                             //Errors which can happen during opening are handled in _tryOpeningInputDevice method
}

void InputDeviceManager::inputDeviceClosureRequested(const InputDeviceInfo& deviceDetails)
{
    for (InputDevice* device : m_openedInputDevices) {
        if (device->deviceInfo() == deviceDetails) {
            device->close();
            return;
        }
    }
}

void InputDeviceManager::_handleAttachedInputDevice(const InputDeviceInfo& deviceDetails)
{
    //This method is invoked when new device is connected to the computer
    if (!inputDeviceManagerSettings->inputDeviceAutoconnection()) {   //If autoconnect disabled
        emit inputDeviceWasAttached(deviceDetails);   //Just inform user about new device
        return;
    }

    if (!inputDeviceManagerSettings->inputDeviceFilter().isMatching(deviceDetails)) {     //If autoconnect enabled, but not matching criteria
        emit inputDeviceWasAttached(deviceDetails);                       //Jsut inform user about new device
        return;
    }

    if (!_tryOpeningInputDevice(deviceDetails)) {     //If autoconnect enabled, criteria are fulfilled, but opening failed
        emit inputDeviceWasAttached(deviceDetails);   //Still just inform user about new device
        return;
    }

    emit inputDeviceWasOpened(deviceDetails);         //Inform user, that device was opened
}

void InputDeviceManager::_handleDetachedInputDevice(const InputDeviceInfo& deviceDetails)
{
    emit inputDeviceWasDetached(deviceDetails);
}

void InputDeviceManager::_handleClosedInputDevice()
{
    InputDevice* device = qobject_cast<InputDevice*>(sender());
    Q_ASSERT(device != nullptr);

    m_openedInputDevices.removeOne(device);

    emit inputDeviceWasClosed(device->deviceInfo());
    device->deleteLater();
}

void InputDeviceManager::_inputDeviceErrorOccured(InputDevice::InputDeviceError error)
{
    //Some device error happened
    InputDevice* device = qobject_cast<InputDevice*>(sender());
    Q_ASSERT(device != nullptr);

    if (error == InputDevice::NoError)
        return;

    emit errorMessage(_getInputErrorMessage(error,device->deviceInfo()));
}

bool InputDeviceManager::_tryOpeningInputDevice(const InputDeviceInfo& deviceDetails)
{
    InputDevice* device = new InputDevice(deviceDetails);
    if (!device->open(InputDevice::ReadOnly)) {
        qDebug() << "Device "<<deviceDetails<<" opening failed. Reason: "<<device->errorString();
        emit errorMessage(_getInputErrorMessage(device->error(),device->deviceInfo()));
        device->deleteLater();
        return false;
    }

    qDebug() << "Device "<<deviceDetails<<" was opened.";
    connect(device,&InputDevice::deviceClosed,this,&InputDeviceManager::_handleClosedInputDevice);
    connect(device,&InputDevice::errorOccured,this,&InputDeviceManager::_inputDeviceErrorOccured);
    connect(device,&InputDevice::keyFound,this,&InputDeviceManager::keyFound);
    m_openedInputDevices.append(device);
    return true;
}

QString InputDeviceManager::_getInputErrorMessage(InputDevice::InputDeviceError error,const InputDeviceInfo& deviceDetails)
{
    switch (error) {
    case InputDevice::NoError:
        Q_ASSERT(false);
    case InputDevice::PermissionError:
        return tr("Check access permissions on device %1").arg(deviceDetails.deviceFilePath());
    case InputDevice::NoSuchDeviceError:
        return tr("Device %1 not found anymore").arg(deviceDetails.deviceFilePath());
    case InputDevice::DeviceIsBusy:
        return tr("Device %1 is busy").arg(deviceDetails.deviceFilePath());
    case InputDevice::UnknownError:
        return tr("Unknown error with device %1").arg(deviceDetails.deviceFilePath());
    }
    Q_ASSERT(false);
    return QString();
}
