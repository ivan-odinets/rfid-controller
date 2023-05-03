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

#ifndef INPUTFILTEREDITDIALOG_H
#define INPUTFILTEREDITDIALOG_H

#include <QDialog>

#include "core/input/InputDeviceFilter.h"

class QComboBox;
class QLineEdit;
class QSpinBox;

/*!
 *  @class InputFilterEditDialog windows/InputFilterEditDialog.h
 *  @brief This dialog allows user to configure InputDeviceFilter objects.
 */

class InputFilterEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputFilterEditDialog(QWidget* parent = nullptr);
    ~InputFilterEditDialog() {}

    void                displayInputFilter(const InputDeviceFilter& filter);
    InputDeviceFilter   inputFilter();

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

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of the code
//

private:
    void           setTargetDeviceNameSet(const DeviceNameSet& set);
    DeviceNameSet  m_deviceNameSet;
    QComboBox*     w_deviceNameSelector;
    QLineEdit*     w_deviceNameEdit;

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
};

#endif // INPUTFILTEREDITDIALOG_H
