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

#include "SerialPortConfig.h"

void SerialPortConfig::configureSerialPort(QSerialPort* port) const
{
    port->setBaudRate(baudRate());
    port->setStopBits(stopBits());
    port->setDataBits(dataBits());
    port->setParity(parity());
    port->setFlowControl(flowControl());
}

SerialPortConfig SerialPortConfig::readFromPort(const QSerialPort& port)
{
    SerialPortConfig result;

    result.setBaudRate(static_cast<QSerialPort::BaudRate>(port.baudRate()));
    result.setStopBits(port.stopBits());
    result.setDataBits(port.dataBits());
    result.setParity(port.parity());
    result.setFlowControl(port.flowControl());

    return result;
}

bool operator==(const QSerialPortInfo& lhs, const QSerialPortInfo& rhs)
{
    return (lhs.portName() == rhs.portName());
}
