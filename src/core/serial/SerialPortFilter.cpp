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

#include "SerialPortFilter.h"

bool SerialPortFilter::isMatching(const QSerialPortInfo& portInfo) const
{
    if (m_portNames.contains(portInfo.portName()))
        return true;

    if (portInfo.hasProductIdentifier() && portInfo.hasVendorIdentifier()) {
        if (!m_vendorIds.contains(portInfo.vendorIdentifier())) {
            return false;
        }

        if (!m_productIds.contains(portInfo.productIdentifier())) {
            return false;
        }

        return true;
    }

    return false;
}

void SerialPortFilter::unite(const SerialPortFilter& filter)
{
    m_vendorIds.unite(filter.vendorIdSet());
    m_productIds.unite(filter.productIdSet());
    m_portNames.unite(filter.portNameSet());
}
