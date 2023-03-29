/*
 **********************************************************************************************************************
 *
 * This file is part of RFID Controller.
 *
 * RFID Controller is free software: you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * RFID Controller is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RFID Controller. If not, see
 * <https://www.gnu.org/licenses/>.
 *
 */

#ifndef SERIALDEVICEMANAGER_H
#define SERIALDEVICEMANAGER_H

#include <QObject>

#include "./SerialPortFilter.h"
#include "./SerialDeviceWatcher.h"
#include "./core/devices/SerialDevice.h"

#include "./appconfig/SerialDeviceManagerSettings.h"

/*!
 *  @class SerialDeviceManager core/serial/SerialDeviceManager.h
 *  @brief This class is responsible for handling Serial devices (manual opening, closure, autoconnection, etc.
 */

class SerialDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialDeviceManager(QObject *parent = nullptr);

    void                     start();
    void                     stop()                             { m_serialDeviceWatcher.stop(); }
    void                     updateSerialDeviceList()           { m_serialDeviceWatcher.updateDeviceList(); }

    QList<QSerialPortInfo>   openedSerialDevices() const;
    QList<QSerialPortInfo>   availableDevices() const           { return QSerialPortInfo::availablePorts(); }

    void                     setDefaultPortConfig(const SerialPortConfig& config);
    SerialPortConfig         defaultPortConfig()                { return p_settings->defaultSerialPortConfiguration(); }

    bool                     serialDeviceAutoconnection() const { return p_settings->serialDeviceAutoconnection(); }
    void                     setSerialDeviceAutoconnection(bool state);

    SerialPortFilter         serialDeviceFilter()               { return p_settings->serialDeviceFilter(); }
    void                     setSerialDeviceFilter(const SerialPortFilter& newFilter);
    void                     appendSerialDeviceFilter(const SerialPortFilter& filter);

signals:
    /*! @brief This signal is emitted when any of the connected devices has read any key/tag/etc. */
    void keyFound(const QString& key);

    /*! @brief This signal is emitted when any error has happeded. Contains description of the error in human-readable
     *         form. */
    void errorMessage(const QString& errorMessage);

    void serialDeviceWasOpened(const QSerialPortInfo& portInfo);
    void serialDeviceWasClosed(const QSerialPortInfo& portInfo);

    void serialDeviceWasAttached(const QSerialPortInfo& portInfo);
    void serialDeviceWasDetached(const QSerialPortInfo& portInfo);

    void deviceAutoconnectChanged(bool newStatus);

public slots:
    void serialDeviceOpeningRequested(const QSerialPortInfo& portInfo);
    void serialDeviceClosureRequested(const QSerialPortInfo& portInfo);

private slots:
    void _handleAttachedSerialDevice(const QSerialPortInfo& portInfo);
    void _handleDetachedSerialDevice(const QSerialPortInfo& portInfo);
    void _handleClosedSerialDevice();
    void _serialDeviceErrorOccured(QSerialPort::SerialPortError error);

private:
    SerialDeviceManagerSettings*  p_settings;
    SerialDeviceWatcher           m_serialDeviceWatcher;
    QList<SerialDevice*>          m_openedSerialDevices;
    bool _tryOpeningSerialDevice(const QSerialPortInfo& portInfo);

    static QString _getSerialErrorMessage(QSerialPort::SerialPortError error,const QSerialPortInfo& portInfo);
};

#endif // SERIALDEVICEMANAGER_H
