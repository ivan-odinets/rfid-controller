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

#include "InputDeviceManagerSettings.h"

#define INPUT_AUTOCONNECT         QStringLiteral("input/autoconnect")
#define HIDE_SYSTEM_HIDS          QStringLiteral("input/hideSystemDevices")
#define INPUT_FILTER_VID          QStringLiteral("input/vendorIdList")
#define INPUT_FILTER_PID          QStringLiteral("input/productIdList")

#if defined(Q_OS_LINUX)
    #define DEVICE_FILE_NAME      QStringLiteral("input/deviceFileNames")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif

InputDeviceManagerSettings* InputDeviceManagerSettings::theOne = nullptr;

InputDeviceManagerSettings::InputDeviceManagerSettings()
{
    Q_ASSERT(theOne == nullptr);
    theOne = this;
}

void InputDeviceManagerSettings::loadValues()
{
    m_inputDeviceAutoconnection = _value(INPUT_AUTOCONNECT).toBool();

    m_inputDeviceFilter.setVendorIdSet(VendorIdSet(_value(INPUT_FILTER_VID,QString()).toString()));
    m_inputDeviceFilter.setProductIdSet(ProductIdSet(_value(INPUT_FILTER_PID,QString()).toString()));
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    m_inputDeviceFilter.setDeviceNameSet(DeviceNameSet(_value(DEVICE_FILE_NAME).toString()));
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
}

void InputDeviceManagerSettings::setInputDeviceAutoconnection(bool state)
{
    m_inputDeviceAutoconnection = state;
    _setValue(INPUT_AUTOCONNECT,state);
}

void InputDeviceManagerSettings::setInputDeviceFilter(const InputDeviceFilter& filter)
{
    setInputProductIdsSet(filter.productIdSet());
    setInputVendorIdsSet(filter.vendorIdSet());
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    setInputDeviceFileNames(filter.deviceNameSet());
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
}

void InputDeviceManagerSettings::appendInputDeviceFilter(const InputDeviceFilter& filter)
{
    m_inputDeviceFilter.unite(filter);

    setInputProductIdsSet(m_inputDeviceFilter.productIdSet());
    setInputVendorIdsSet(m_inputDeviceFilter.vendorIdSet());
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    setInputDeviceFileNames(m_inputDeviceFilter.deviceNameSet());
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
}

void InputDeviceManagerSettings::setInputVendorIdsSet(const VendorIdSet& newVendorIds)
{
    m_inputDeviceFilter.setVendorIdSet(newVendorIds);
    _setValue(INPUT_FILTER_VID,newVendorIds.toString());
}

void InputDeviceManagerSettings::setInputProductIdsSet(const VendorIdSet& newProductIds)
{
    m_inputDeviceFilter.setProductIdSet(newProductIds);
    _setValue(INPUT_FILTER_PID,newProductIds.toString());
}

/*
 **********************************************************************************************************************
 * Linux-specific code
 */

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)

void InputDeviceManagerSettings::setInputDeviceFileNames(const DeviceNameSet& deviceNames)
{
    m_inputDeviceFilter.setDeviceNameSet(deviceNames);
    _setValue(DEVICE_FILE_NAME,deviceNames.toString());
}

#endif //PLATFORM SPECIFIC
