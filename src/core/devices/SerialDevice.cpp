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

#include "SerialDevice.h"

#include <QDebug>

SerialDevice::SerialDevice(QObject* parent)
    : QObject(parent)
{}
SerialDevice::SerialDevice(const QSerialPortInfo& portInfo, QObject* parent)
    : QObject{parent},m_portInfo(portInfo)
{}

SerialDevice::~SerialDevice()
{
    close();
}

bool SerialDevice::open(QIODevice::OpenMode openMode)
{
    m_port.setPort(m_portInfo);

    if (!m_port.open(openMode)) {
        emit errorOccured(m_port.error());
        return false;
    }

    connect(&m_port,&QSerialPort::readyRead,this,&SerialDevice::_portReadyRead);
    connect(&m_port,&QSerialPort::errorOccurred,this,&SerialDevice::_portError);
    return true;
}

void SerialDevice::close()
{
    disconnect(&m_port,&QSerialPort::readyRead,this,&SerialDevice::_portReadyRead);
    disconnect(&m_port,&QSerialPort::errorOccurred,this,&SerialDevice::_portError);
    m_port.close();
    emit deviceClosed();
}

void SerialDevice::_portReadyRead()
{
    _processSerialInput(m_port.readAll());
}

void SerialDevice::_portError(QSerialPort::SerialPortError error)
{
    //If we have something wrong - close the device
    close();
    //And inform anybody about this
    emit errorOccured(error);
}

void SerialDevice::_processSerialInput(const QByteArray& bytes)
{
    QString key = bytes;
    key.remove('\r');
    key.remove('\n');
    emit keyFound(key);
}