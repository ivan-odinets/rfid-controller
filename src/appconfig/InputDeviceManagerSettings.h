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

    void                loadValues();

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
    InputDeviceManagerSettings();

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

#endif // INPUTDEVICEMANAGERSETTINGS_H
