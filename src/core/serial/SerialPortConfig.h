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

#ifndef SERIALPORTCONFIG_H
#define SERIALPORTCONFIG_H

#include <QSerialPort>
#include <QSerialPortInfo>

/*!
 *  @class SerialPortConfig core/serial/SerialPortConfig.h
 *  @brief This class holds information about QSerialPort configuration ooptions. Like baud rate, data bits etc.
 */

class SerialPortConfig
{
public:
    QSerialPort::BaudRate         baudRate() const                   { return m_baudRate; }
    QSerialPort::StopBits         stopBits() const                   { return m_stopBits; }
    QSerialPort::DataBits         dataBits() const                   { return m_dataBits; }
    QSerialPort::Parity           parity() const                     { return m_parity; }
    QSerialPort::FlowControl      flowControl() const                { return m_flowControl; }

    void setBaudRate(QSerialPort::BaudRate rate)                     { m_baudRate = rate; };
    void setStopBits(QSerialPort::StopBits stopBits)                 { m_stopBits = stopBits; };
    void setDataBits(QSerialPort::DataBits dataBits)                 { m_dataBits = dataBits; };
    void setParity(QSerialPort::Parity parity)                       { m_parity = parity; };
    void setFlowControl(QSerialPort::FlowControl flowControl)        { m_flowControl = flowControl; }

    /*! @brief This method is used to apply configuration stored in this object to QSerialPort passed as a pointer */
    void                          configureSerialPort(QSerialPort* port) const;

    /*! @brief This method is used to read configuration from QSerialPort and construct new SerialPortConfig object */
    static SerialPortConfig       readFromPort(const QSerialPort& port);

private:
    QSerialPort::BaudRate    m_baudRate;
    QSerialPort::StopBits    m_stopBits;
    QSerialPort::DataBits    m_dataBits;
    QSerialPort::Parity      m_parity;
    QSerialPort::FlowControl m_flowControl;
};

bool          operator==(const QSerialPortInfo& lhs, const QSerialPortInfo& rhs);

#endif // SERIALPORTCONFIG_H
