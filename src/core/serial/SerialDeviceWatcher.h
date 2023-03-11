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

    void start();
    void stop();
    void updateDeviceList();

    void setAutoupdateInterval(uint newInterval);
    uint autoupdateInterval() const                        { return m_autoupdateInterval; }

    QList<QSerialPortInfo> getAvailableDevices() const     { return m_lastScanResults; }

signals:
    void deviceWasAttached(const QSerialPortInfo& deviceDetails);
    void deviceWasDetached(const QSerialPortInfo& deviceDetails);

private:
    QList<QSerialPortInfo>   m_lastScanResults;
    QTimer                   m_autoupdateTimer;
    uint                     m_autoupdateInterval;
};

#endif // SERIALDEVICEWATCHER_H
