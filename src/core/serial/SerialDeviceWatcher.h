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

#ifndef SERIALDEVICEWATCHER_H
#define SERIALDEVICEWATCHER_H

#include <QObject>

#include <QTimer>
#include <QSerialPortInfo>

/*!
 * @class SerialDeviceWatcher core/serial/SerialDeviceWatcher.h
 * @brief This class is responsible for monitoring system for newly attached/or detached serial devices.
 * @details Emitting signal SerialDeviceWatcher::deviceWasAttached when some Serial device is atatched.
 *          And signal SerialDeviceWatcher::deviceWasDetached when HID device was detached.
 */


class SerialDeviceWatcher : public QObject
{
    Q_OBJECT
public:
    explicit SerialDeviceWatcher(QObject *parent = nullptr);
    ~SerialDeviceWatcher();

    /*! @brief Use this method to start automated updating of serial device list. */
    void start();

    /*! @brief Use this method to stop automated updating of serial device list. */
    void stop();

    /*! @brief use this method to manual update of serial device list. */
    void updateDeviceList();

    /*! @brief Use this method to set autoupdate interval (in milliseconds) */
    void setAutoupdateInterval(uint newInterval);

    /*! @brief Use this method to get current autoupdate interval. */
    uint autoupdateInterval() const                        { return m_autoupdateInterval; }

    /*! @brief This method returns list of serial devices, which are currently attached to the system. */
    QList<QSerialPortInfo> availableDevices() const        { return m_lastScanResults; }

signals:
    /*! @brief This signal is emitted when new Serial device is attached. Note that the device is considered as
     *         "new" when it was attached after calling SerialDeviceWatcher::start. */
    void deviceWasAttached(const QSerialPortInfo& deviceDetails);

    /*! @brief This signal is emitted when new Serial device is detached. Note that the device is considered as
     *         "removed" when it was attached after calling SerialDeviceWatcher::start. */
    void deviceWasDetached(const QSerialPortInfo& deviceDetails);

private:
    QList<QSerialPortInfo>   m_lastScanResults;
    QTimer                   m_autoupdateTimer;
    uint                     m_autoupdateInterval;
};

#endif // SERIALDEVICEWATCHER_H
