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

#ifndef SERIALFILTEREDITDIALOG_H
#define SERIALFILTEREDITDIALOG_H

#include <QDialog>

#include "core/serial/SerialPortFilter.h"

class QComboBox;
class QLineEdit;
class QSpinBox;

/*!
 *  @class SerialFilterEditDialog windows/SerialFilterEditDialog.h
 *  @brief This dialog allows user to configure SerialPortFilter objects.
 */

class SerialFilterEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SerialFilterEditDialog(QWidget* parent = nullptr);
    ~SerialFilterEditDialog() {}

    void                displaySerialFilter(const SerialPortFilter& filter);
    SerialPortFilter    serialFilter();

private:
    inline void _setupUi();

    //VendorId
    void           setTargetVendorIdSet(const VendorIdSet& set);
    VendorIdSet    m_vendorIdSet;
    QComboBox*     w_vendorIdSelector;
    QSpinBox*      w_vendorIdSpinBox;

    //ProductId
    void           setTargetProductIdSet(const ProductIdSet& set);
    ProductIdSet   m_productIdSet;
    QComboBox*     w_productIdSelector;
    QSpinBox*      w_productIdSpinBox;

    //Device names
    void           setTargetDeviceNameSet(const DeviceNameSet& set);
    DeviceNameSet  m_deviceNameSet;
    QComboBox*     w_deviceNameSelector;
    QLineEdit*     w_deviceNameEdit;
};

#endif // SERIALFILTEREDITDIALOG_H
