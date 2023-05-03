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

#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>

#include "./core/serial/SerialPortConfig.h"

/*!
 * @class SerialDevice devices/SerialDevice.h
 * @brief Represents an serial device.
 */

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject* parent = nullptr);
    explicit SerialDevice(const QSerialPortInfo& portInfo, QObject* parent = nullptr);
    ~SerialDevice();

    bool open(QIODevice::OpenMode openMode);
    void close();
    bool isOpened() const;

    void setPortInfo(const QSerialPortInfo& portInfo)                { m_portInfo = portInfo; }
    void configureSerialPort(const SerialPortConfig& config)         { config.configureSerialPort(&m_port); }
    QSerialPortInfo portInfo() const                                 { return m_portInfo; }

    QSerialPort::SerialPortError error() const                       { return m_port.error(); }
    QString errorString() const                                      { return m_port.errorString(); }

signals:
    void keyFound(const QString& key);

    void deviceClosed();
    void errorOccured(QSerialPort::SerialPortError error);
    void disconnected();

protected slots:
    void _portReadyRead();
    void _portError(QSerialPort::SerialPortError error);

protected:
    void  _processSerialInput(const QByteArray& bytes);

private:
    QSerialPort         m_port;
    QSerialPortInfo     m_portInfo;
};

#endif // SERIALDEVICE_H
