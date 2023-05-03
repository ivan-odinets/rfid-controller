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

#ifndef INPUTDEVICEMANAGERSETTINGS_H
#define INPUTDEVICEMANAGERSETTINGS_H

#include "./SettingsCore.h"

#include "./core/ServiceTypes.h"
#include "./core/input/InputDeviceFilter.h"

class InputDeviceManagerSettings : public virtual SettingsCore
{
public:
    static InputDeviceManagerSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    bool                inputDeviceAutoconnection() const  { return m_inputDeviceAutoconnection; }
    void                setInputDeviceAutoconnection(bool state);

    InputDeviceFilter   inputDeviceFilter() const          { return m_inputDeviceFilter; }
    void                setInputDeviceFilter(const InputDeviceFilter& filter);
    void                appendInputDeviceFilter(const InputDeviceFilter& filter);

    VendorIdSet         inputVendorIdsSet() const          { return m_inputDeviceFilter.vendorIdSet(); }
    void                setInputVendorIdsSet(const VendorIdSet& newVendorIds);

    ProductIdSet        inputProductIdsSet() const         { return m_inputDeviceFilter.productIdSet(); }
    void                setInputProductIdsSet(const ProductIdSet& newVendorIds);

protected:
    InputDeviceManagerSettings() {
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }
    void _loadValues();

private:
    static InputDeviceManagerSettings* theOne;

    bool                m_inputDeviceAutoconnection;
    InputDeviceFilter   m_inputDeviceFilter;

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of DeviceManagerSettings
//

public:
    DeviceNameSet       inputDeviceFileNames() const       { return m_inputDeviceFilter.deviceNameSet(); }
    void                setInputDeviceFileNames(const DeviceNameSet& deviceNames);

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
};
#define inputDeviceManagerSettings InputDeviceManagerSettings::get()

#endif // INPUTDEVICEMANAGERSETTINGS_H
