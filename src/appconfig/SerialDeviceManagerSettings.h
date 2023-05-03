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

#ifndef SERIALDEVICEMANAGERSETTINGS_H
#define SERIALDEVICEMANAGERSETTINGS_H

#include "./SettingsCore.h"

#include <QSerialPort>

#include "./core/serial/SerialPortConfig.h"
#include "./core/serial/SerialPortFilter.h"

class SerialDeviceManagerSettings : public virtual SettingsCore
{
public:
    static SerialDeviceManagerSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    SerialPortConfig    defaultSerialPortConfiguration() const { return m_serialPortConfig; }
    void                setDefaultSerialPortCongiguration(const SerialPortConfig& params);

    bool                serialDeviceAutoconnection() const { return m_serialDeviceAutoconnection; }
    void                setSerialDeviceAutoconnection(bool state);

    SerialPortFilter    serialDeviceFilter() const { return m_serialPortFilter; }
    void                saveSerialDeviceFilter(const SerialPortFilter& filter);
    void                appendSerialDeviceFilter(const SerialPortFilter& filter);

protected:
    SerialDeviceManagerSettings() {
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }
    void _loadValues();

private:
    static SerialDeviceManagerSettings* theOne;

    SerialPortConfig    m_serialPortConfig;
    bool                m_serialDeviceAutoconnection;
    SerialPortFilter    m_serialPortFilter;
};
#define serialDeviceManagerSettings SerialDeviceManagerSettings::get()

#endif // SERIALDEVICEMANAGERSETTINGS_H
