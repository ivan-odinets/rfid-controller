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

#include "SerialFilterEditDialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "widgets/Hex16BitSpinBox.h"

SerialFilterEditDialog::SerialFilterEditDialog(QWidget* parent)
    : QDialog(parent)
{
    _setupUi();
}

void SerialFilterEditDialog::displaySerialFilter(const SerialPortFilter& filter)
{
    setTargetVendorIdSet(filter.vendorIdSet());
    setTargetProductIdSet(filter.productIdSet());
    setTargetDeviceNameSet(filter.portNameSet());
}

SerialPortFilter SerialFilterEditDialog::serialFilter()
{
    SerialPortFilter result;

    result.setVendorIdsSet(m_vendorIdSet);
    result.setProductIdsSet(m_productIdSet);
    result.setPortNameSet(m_deviceNameSet);

    return result;
}

void SerialFilterEditDialog::setTargetVendorIdSet(const VendorIdSet& vendorIdSet)
{
    m_vendorIdSet = vendorIdSet;
    foreach (ProductId vendorId, vendorIdSet) {
        w_vendorIdSelector->addItem(QString::number(vendorId,16),vendorId);
    }
}

void SerialFilterEditDialog::setTargetProductIdSet(const ProductIdSet& productIdSet)
{
    m_productIdSet = productIdSet;
    foreach (ProductId productId, productIdSet) {
        w_productIdSelector->addItem(QString::number(productId,16),productId);
    }
}

void SerialFilterEditDialog::setTargetDeviceNameSet(const DeviceNameSet& deviceNames)
{
    m_deviceNameSet = deviceNames;
    foreach (QString deviceName, deviceNames) {
        w_deviceNameSelector->addItem(deviceName,deviceName);
    }
}

/*
 **********************************************************************************************************************
 * Configure UI
 */

void SerialFilterEditDialog::_setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QGridLayout* paramsLayout = new QGridLayout;

    // Configure VendorIdSet editing
    paramsLayout->addWidget(new QLabel(tr("Vendor ID:")),0,0);
    w_vendorIdSpinBox = new Hex16BitSpinBox;
    paramsLayout->addWidget(w_vendorIdSpinBox,0,1);

    w_vendorIdSelector = new QComboBox;
    w_vendorIdSelector->addItem(tr("<Add new vendor_id>"));
    connect(w_vendorIdSelector,qOverload<int>(&QComboBox::currentIndexChanged),
            [=](int index){ w_vendorIdSpinBox->setValue(w_vendorIdSelector->itemData(index).toUInt()); }
    );
    paramsLayout->addWidget(w_vendorIdSelector,0,2);


    QPushButton* addVendorButton = new QPushButton("+");
    addVendorButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(addVendorButton,&QPushButton::clicked,[this](){
        int selectedIndex = w_vendorIdSelector->currentIndex();
        if (selectedIndex != 0)
            return;

        VendorId vendorId = w_vendorIdSpinBox->value();
        m_vendorIdSet.insert(vendorId);
        w_vendorIdSelector->addItem(QString::number(vendorId,16),vendorId);
    });
    paramsLayout->addWidget(addVendorButton,0,3);

    QPushButton* removeVendorButton = new QPushButton("-");
    removeVendorButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(removeVendorButton,&QPushButton::clicked,[this](){
        int selectedIndex = w_vendorIdSelector->currentIndex();
        if (selectedIndex == 0)
            return;

        m_vendorIdSet.remove(w_vendorIdSelector->currentData().toUInt());
        w_vendorIdSelector->removeItem(selectedIndex);
    });
    paramsLayout->addWidget(removeVendorButton,0,4);

    // Configure ProductIdSet editing
    paramsLayout->addWidget(new QLabel(tr("Product ID:")),1,0);
    w_productIdSpinBox = new Hex16BitSpinBox;
    paramsLayout->addWidget(w_productIdSpinBox,1,1);

    w_productIdSelector = new QComboBox;
    w_productIdSelector->addItem(tr("<Add new product_id>"));
    connect(w_productIdSelector,qOverload<int>(&QComboBox::currentIndexChanged),
            [=](int index){ w_productIdSpinBox->setValue(w_productIdSelector->itemData(index).toUInt()); }
    );
    paramsLayout->addWidget(w_productIdSelector,1,2);

    QPushButton* addProductButton = new QPushButton("+");
    addProductButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(addProductButton,&QPushButton::clicked,[this](){
        int selectedIndex = w_productIdSelector->currentIndex();
        if (selectedIndex != 0)
            return;

        ProductId productId = w_productIdSpinBox->value();
        m_productIdSet.insert(productId);
        w_productIdSelector->addItem(QString::number(productId,16),productId);
    });
    paramsLayout->addWidget(addProductButton,1,3);

    QPushButton* removeProductButton = new QPushButton("-");
    removeProductButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(removeProductButton,&QPushButton::clicked,[this](){
        int selectedIndex = w_productIdSelector->currentIndex();
        if (selectedIndex == 0)
            return;

        m_productIdSet.remove(w_productIdSelector->currentData().toUInt());
        w_productIdSelector->removeItem(selectedIndex);
    });
    paramsLayout->addWidget(removeProductButton,1,4);

    // Configure DeviceFileNameSet editing
    paramsLayout->addWidget(new QLabel(tr("Device name:")),2,0);
    w_deviceNameEdit = new QLineEdit;
    paramsLayout->addWidget(w_deviceNameEdit,2,1);

    w_deviceNameSelector = new QComboBox;
    w_deviceNameSelector->addItem(tr("<Add new device name>"));
    connect(w_deviceNameSelector,qOverload<int>(&QComboBox::currentIndexChanged),
            [=](int index){ w_deviceNameEdit->setText(w_deviceNameSelector->itemData(index).toString()); }
    );
    paramsLayout->addWidget(w_deviceNameSelector,2,2);

    QPushButton* addDeviceNameButton = new QPushButton("+");
    addDeviceNameButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(addDeviceNameButton,&QPushButton::clicked,[this](){
        int selectedIndex = w_deviceNameSelector->currentIndex();
        if (selectedIndex != 0)
            return;

        QString deviceName = w_deviceNameEdit->text();
        m_deviceNameSet.insert(deviceName);
        w_deviceNameSelector->addItem(deviceName,deviceName);
    });
    paramsLayout->addWidget(addDeviceNameButton,2,3);

    QPushButton* removeDeviceNameButton = new QPushButton("-");
    removeDeviceNameButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    connect(removeDeviceNameButton,&QPushButton::clicked,[this](){
        int selectedIndex = w_deviceNameSelector->currentIndex();
        if (selectedIndex == 0)
            return;

        m_deviceNameSet.remove(w_deviceNameSelector->currentData().toString());
        w_deviceNameSelector->removeItem(selectedIndex);
    });
    paramsLayout->addWidget(removeDeviceNameButton,2,4);

    mainLayout->addLayout(paramsLayout);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttons,&QDialogButtonBox::accepted,this,&SerialFilterEditDialog::accept);
    connect(buttons,&QDialogButtonBox::rejected,this,&SerialFilterEditDialog::reject);
    mainLayout->addWidget(buttons);

    setLayout(mainLayout);
}


