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

#include "InputDeviceFilter.h"

bool InputDeviceFilter::isMatching(const InputDeviceInfo& deviceInfo)
{
    if (m_productIds.contains(deviceInfo.productId()) &&
        m_vendorIds.contains(deviceInfo.vendorId()))
        return true;

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    if (m_deviceNames.contains(deviceInfo.deviceFileName()))
        return true;
#endif //PLATFORM SPECIFIC

    //Nothing is fitting?
    return false;
}

void InputDeviceFilter::unite(const InputDeviceFilter& filter)
{
    m_vendorIds.unite(filter.vendorIdSet());
    m_productIds.unite(filter.productIdSet());

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    m_deviceNames.unite(filter.deviceNameSet());
#endif //PLATFORM SPECIFIC
}

void InputDeviceFilter::setProductIdSet(const ProductIdSet& newProductIds)
{
    m_productIds = newProductIds;
}

void InputDeviceFilter::addProductId(ProductId newProductId)
{
    m_productIds.insert(newProductId);
}

void InputDeviceFilter::removeProductId(ProductId productId)
{
    m_productIds.remove(productId);
}

void InputDeviceFilter::setVendorIdSet(const VendorIdSet& newVendorIds)
{
    m_vendorIds = newVendorIds;
}

void InputDeviceFilter::addVendorId(VendorId newVendorId)
{
    m_vendorIds.insert(newVendorId);
}

void InputDeviceFilter::removeVendorId(VendorId vendorId)
{
    m_vendorIds.remove(vendorId);
}

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of InputDeviceFilter
//

void InputDeviceFilter::setDeviceNameSet(const DeviceNameSet& newDeviceNames)
{
    m_deviceNames = newDeviceNames;
}

void InputDeviceFilter::addDeviceName(const QString& newDeviceName)
{
    m_deviceNames.insert(newDeviceName);
}

void InputDeviceFilter::removeDeviceName(const QString& deviceName)
{
    m_deviceNames.remove(deviceName);
}

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
