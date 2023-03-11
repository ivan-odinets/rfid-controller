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

#include "SerialDeviceManagerSettings.h"

#define DEFAULT_BAUD_RATE           QStringLiteral("serial/defaultBaudRate")
#define DEFAULT_DATA_BITS           QStringLiteral("serial/defaultDataBits")
#define DEFAULT_FLOW_CONTROL        QStringLiteral("serial/defaultFlowControl")
#define DEFAULT_PARITY              QStringLiteral("serial/defaultParity")
#define DEFAULT_STOP_BITS           QStringLiteral("serial/defaultStopBits")

#define SERIAL_AUTOCONNECT          QStringLiteral("serial/autoconnect")
#define SERIAL_PORT_NAMES           QStringLiteral("serial/portNames")
#define SERIAL_FILTER_VID           QStringLiteral("serial/vendorIdList")
#define SERIAL_FILTER_PID           QStringLiteral("serial/productIdList")

SerialDeviceManagerSettings* SerialDeviceManagerSettings::theOne = nullptr;

SerialDeviceManagerSettings::SerialDeviceManagerSettings()
{
    Q_ASSERT(theOne == nullptr);
    theOne = this;
}

void SerialDeviceManagerSettings::loadValues()
{
    m_serialPortConfig.setBaudRate(_value(DEFAULT_BAUD_RATE,QSerialPort::Baud9600).value<QSerialPort::BaudRate>());
    m_serialPortConfig.setDataBits(_value(DEFAULT_DATA_BITS,QSerialPort::Data5).value<QSerialPort::DataBits>());
    m_serialPortConfig.setFlowControl(_value(DEFAULT_FLOW_CONTROL,QSerialPort::SoftwareControl).value<QSerialPort::FlowControl>());
    m_serialPortConfig.setParity(_value(DEFAULT_PARITY,QSerialPort::NoParity).value<QSerialPort::Parity>());
    m_serialPortConfig.setStopBits(_value(DEFAULT_STOP_BITS,QSerialPort::OneStop).value<QSerialPort::StopBits>());

    m_serialDeviceAutoconnection = _value(SERIAL_AUTOCONNECT).toBool();

    m_serialPortFilter.setPortNameSet(SerialPortNamesSet(_value(SERIAL_PORT_NAMES).toString()));
    m_serialPortFilter.setVendorIdsSet(VendorIdSet(_value(SERIAL_FILTER_VID).toString()));
    m_serialPortFilter.setVendorIdsSet(ProductIdSet(_value(SERIAL_FILTER_PID).toString()));
}

void SerialDeviceManagerSettings::setDefaultSerialPortCongiguration(const SerialPortConfig& params)
{
    m_serialPortConfig = params;
    _setValue(DEFAULT_BAUD_RATE,params.baudRate());
    _setValue(DEFAULT_DATA_BITS,params.dataBits());
    _setValue(DEFAULT_FLOW_CONTROL,params.flowControl());
    _setValue(DEFAULT_PARITY,params.parity());
    _setValue(DEFAULT_STOP_BITS,params.stopBits());
}

void SerialDeviceManagerSettings::setSerialDeviceAutoconnection(bool state)
{
    m_serialDeviceAutoconnection = state;
    _setValue(SERIAL_AUTOCONNECT,state);
}

void SerialDeviceManagerSettings::saveSerialDeviceFilter(const SerialPortFilter& filter)
{
    m_serialPortFilter = filter;
    _setValue(SERIAL_PORT_NAMES,m_serialPortFilter.portNameSet().toString());
    _setValue(SERIAL_FILTER_VID,m_serialPortFilter.vendorIdSet().toString());
    _setValue(SERIAL_FILTER_PID,m_serialPortFilter.productIdSet().toString());
}

void SerialDeviceManagerSettings::appendSerialDeviceFilter(const SerialPortFilter& filter)
{
    m_serialPortFilter.unite(filter);
    _setValue(SERIAL_PORT_NAMES,m_serialPortFilter.portNameSet().toString());
    _setValue(SERIAL_FILTER_VID,m_serialPortFilter.vendorIdSet().toString());
    _setValue(SERIAL_FILTER_PID,m_serialPortFilter.productIdSet().toString());
}
