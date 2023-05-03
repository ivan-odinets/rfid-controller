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

#ifndef SERIALPORTFILTER_H
#define SERIALPORTFILTER_H

#include <QSerialPortInfo>

#include "core/ServiceTypes.h"

/*!
 *  @class SerialPortFilter core/serial/SerialPortFilter.h
 *  @brief This class is responsible for checking if serial device is matching specified criteria
 *         (vendorId, productId, or device file nameю
 */

class SerialPortFilter
{
public:
    SerialPortFilter() {}
    ~SerialPortFilter() {}

    /*! @brief Returns true if QSerialPortInfo is matching specified criteria */
    bool isMatching(const QSerialPortInfo& portInfo) const;

    /*! @brief This method can be used to merge this object with another SerialPortFilter object */
    void unite(const SerialPortFilter& filter);

    void                setPortNameSet(const SerialPortNamesSet& newDeviceNames)    { m_portNames = newDeviceNames; }
    void                addPortName(const QString& newDeviceName);
    void                removePortName(const QString& deviceName);
    SerialPortNamesSet  portNameSet() const          { return m_portNames; }

    void           setVendorIdsSet(const VendorIdSet& newVendorIds)         { m_vendorIds = newVendorIds; }
    VendorIdSet    vendorIdSet() const                                      { return m_vendorIds; }

    void           setProductIdsSet(const ProductIdSet& newProductIds)      { m_productIds = newProductIds; }
    ProductIdSet   productIdSet() const                                     { return m_productIds; }

private:
    SerialPortNamesSet  m_portNames;
    VendorIdSet         m_vendorIds;
    ProductIdSet        m_productIds;
};

#endif // SERIALPORTFILTER_H
