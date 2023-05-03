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

#include "SerialDeviceManagerSettings.h"

static const QLatin1String DEFAULT_BAUD_RATE(    "serial/defaultBaudRate"      );
static const QLatin1String DEFAULT_DATA_BITS(    "serial/defaultDataBits"      );
static const QLatin1String DEFAULT_FLOW_CONTROL( "serial/defaultFlowControl"   );
static const QLatin1String DEFAULT_PARITY(       "serial/defaultParity"        );
static const QLatin1String DEFAULT_STOP_BITS(    "serial/defaultStopBits"      );

static const QLatin1String SERIAL_AUTOCONNECT(   "serial/autoconnect");
static const QLatin1String SERIAL_PORT_NAMES(    "serial/portNames");
static const QLatin1String SERIAL_FILTER_VID(    "serial/vendorIdList");
static const QLatin1String SERIAL_FILTER_PID(    "serial/productIdList");

SerialDeviceManagerSettings* SerialDeviceManagerSettings::theOne = nullptr;

void SerialDeviceManagerSettings::_loadValues()
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
