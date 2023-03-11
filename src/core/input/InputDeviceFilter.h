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

#ifndef INPUTDEVICEFILTER_H
#define INPUTDEVICEFILTER_H

#include <QObject>

#include "InputDeviceInfo.h"
#include "core/ServiceTypes.h"

/*!
 * @class InputDeviceFilter core/input/InputDeviceFilter.h
 * @brief This class is responsible for checking if input device is matching specified criteria
 *        (vendorId, productId, or device file name (Linux only)
 */

class InputDeviceFilter
{
public:
    InputDeviceFilter() {}
    ~InputDeviceFilter() {}

    /*! @brief Returns true if InputDeviceInfo is matching specified criteria */
    bool isMatching(const InputDeviceInfo& deviceInfo);

    /*! @brief This method can be used to merge this object with another InputDeviceFilter object */
    void unite(const InputDeviceFilter& filter);

    void           setProductIdSet(const ProductIdSet& newProductIds);
    void           addProductId(ProductId newProductId);
    void           removeProductId(ProductId productId);
    ProductIdSet   productIdSet() const           { return m_productIds; }

    void           setVendorIdSet(const ProductIdSet& newVendorIds);
    void           addVendorId(VendorId newVendorId);
    void           removeVendorId(VendorId vendorId);
    VendorIdSet    vendorIdSet() const            { return m_vendorIds; }

private:
    ProductIdSet           m_productIds;
    VendorIdSet            m_vendorIds;

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of InputDeviceFilter
//

public:
    void           setDeviceNameSet(const DeviceNameSet& newDeviceNames);
    void           addDeviceName(const QString& newDeviceName);
    void           removeDeviceName(const QString& deviceName);
    DeviceNameSet  deviceNameSet() const          { return m_deviceNames; }
private:
    DeviceNameSet          m_deviceNames;

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
};



#endif // INPUTDEVICEFILTER_H
