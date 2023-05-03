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
