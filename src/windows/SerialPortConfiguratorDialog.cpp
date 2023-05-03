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

#include "SerialPortConfiguratorDialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>

SerialPortConfiguratorDialog::SerialPortConfiguratorDialog(QWidget* parent)
    : QDialog(parent)
{
    _setupUi();
}

void SerialPortConfiguratorDialog::displaySerialPortConfig(const SerialPortConfig& config)
{
    setBaudRate(config.baudRate());
    setStopBits(config.stopBits());
    setDataBits(config.dataBits());
    setParity(config.parity());
    setFlowControl(config.flowControl());
}

SerialPortConfig SerialPortConfiguratorDialog::serialPortConfig() const
{
    SerialPortConfig result;

    result.setBaudRate(baudRate());
    result.setStopBits(stopBits());
    result.setDataBits(dataBits());
    result.setParity(parity());
    result.setFlowControl(flowControl());

    return result;
}

/*
 **********************************************************************************************************************
 * Getters
 */

QSerialPort::BaudRate SerialPortConfiguratorDialog::baudRate() const
{
    return w_baudSelector->currentData().value<QSerialPort::BaudRate>();
}

QSerialPort::StopBits SerialPortConfiguratorDialog::stopBits() const
{
    return w_stopBitsSelector->currentData().value<QSerialPort::StopBits>();
}

QSerialPort::DataBits SerialPortConfiguratorDialog::dataBits() const
{
    return w_dataBitsSelector->currentData().value<QSerialPort::DataBits>();
}

QSerialPort::Parity SerialPortConfiguratorDialog::parity() const
{
    return w_paritySelector->currentData().value<QSerialPort::Parity>();
}

QSerialPort::FlowControl SerialPortConfiguratorDialog::flowControl() const
{
    return w_flowControlSelector->currentData().value<QSerialPort::FlowControl>();
}

/*
 **********************************************************************************************************************
 * Setters
 */

void SerialPortConfiguratorDialog::setBaudRate(QSerialPort::BaudRate rate)
{
    _selectValue(w_baudSelector,rate);
}

void SerialPortConfiguratorDialog::setStopBits(QSerialPort::StopBits stopBits)
{
    _selectValue(w_stopBitsSelector,stopBits);
}

void SerialPortConfiguratorDialog::setDataBits(QSerialPort::DataBits dataBits)
{
    _selectValue(w_dataBitsSelector,dataBits);
}

void SerialPortConfiguratorDialog::setParity(QSerialPort::Parity parity)
{
    _selectValue(w_paritySelector,parity);
}

void SerialPortConfiguratorDialog::setFlowControl(QSerialPort::FlowControl flowControl)
{
    _selectValue(w_flowControlSelector,flowControl);
}

template<class C>
void SerialPortConfiguratorDialog::_selectValue(QComboBox* box,C value)
{
    for (int i = 0; i < box->count(); i++) {
        if (box->itemData(i).value<C>() == value) {
            box->setCurrentIndex(i);
            break;
        }
    }
}

void SerialPortConfiguratorDialog::_setupUi()
{
    QGridLayout* mainLayout = new QGridLayout;

    mainLayout->addWidget(new QLabel(tr("Baud rate")),0,0);
    w_baudSelector = _createBaudSelector();
    mainLayout->addWidget(w_baudSelector,0,1);

    mainLayout->addWidget(new QLabel(tr("Stop Bits")),1,0);
    w_stopBitsSelector = _createStopBitsSelector();
    mainLayout->addWidget(w_stopBitsSelector,1,1);

    mainLayout->addWidget(new QLabel(tr("Data Bits")),2,0);
    w_dataBitsSelector = _createDataBitsSelector();
    mainLayout->addWidget(w_dataBitsSelector,2,1);

    mainLayout->addWidget(new QLabel(tr("Parity")),3,0);
    w_paritySelector = _createParitySelector();
    mainLayout->addWidget(w_paritySelector);

    mainLayout->addWidget(new QLabel(tr("Flow Control")),4,0);
    w_flowControlSelector = _createFlowControlSelector();
    mainLayout->addWidget(w_flowControlSelector);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttons,&QDialogButtonBox::accepted,this,&SerialPortConfiguratorDialog::accept);
    connect(buttons,&QDialogButtonBox::rejected,this,&SerialPortConfiguratorDialog::reject);
    mainLayout->addWidget(buttons,5,0,1,2);

    mainLayout->setRowStretch(6,1);

    setLayout(mainLayout);
}

QComboBox* SerialPortConfiguratorDialog::_createBaudSelector()
{
    QComboBox* result = new QComboBox;

    result->addItem(tr("Baud 1200"),QSerialPort::Baud1200);
    result->addItem(tr("Baud 2400"),QSerialPort::Baud2400);
    result->addItem(tr("Baud 4800"),QSerialPort::Baud4800);
    result->addItem(tr("Baud 9600"),QSerialPort::Baud9600);
    result->addItem(tr("Baud 19200"),QSerialPort::Baud19200);
    result->addItem(tr("Baud 38400"),QSerialPort::Baud38400);
    result->addItem(tr("Baud 57600"),QSerialPort::Baud57600);
    result->addItem(tr("Baud 115200"),QSerialPort::Baud115200);
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    result->addItem(tr("Unknown Baud"),QSerialPort::UnknownBaud);
#endif //Only for Qt < 5.15
    return result;
}

QComboBox* SerialPortConfiguratorDialog::_createStopBitsSelector()
{
    QComboBox* result = new QComboBox;

    result->addItem(tr("One Stop"),QSerialPort::OneStop);
    result->addItem(tr("One And Half Stop"),QSerialPort::OneAndHalfStop);
    result->addItem(tr("Two Stop"),QSerialPort::TwoStop);
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    result->addItem(tr("Unknown Stop Bits"),QSerialPort::UnknownStopBits);
#endif //Only for Qt < 5.15

    return result;
}

QComboBox* SerialPortConfiguratorDialog::_createDataBitsSelector()
{
    QComboBox* result = new QComboBox;

    result->addItem(tr("Data 5"),QSerialPort::Data5);
    result->addItem(tr("Data 6"),QSerialPort::Data6);
    result->addItem(tr("Data 7"),QSerialPort::Data7);
    result->addItem(tr("Data 8"),QSerialPort::Data8);
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    result->addItem(tr("Unknown data bits"),QSerialPort::UnknownDataBits);
#endif //Only for Qt < 5.15

    return result;
}

QComboBox* SerialPortConfiguratorDialog::_createParitySelector()
{
    QComboBox* result = new QComboBox;

    result->addItem(tr("No Parity"),QSerialPort::NoParity);
    result->addItem(tr("Even Parity"),QSerialPort::EvenParity);
    result->addItem(tr("Odd Parity"),QSerialPort::OddParity);
    result->addItem(tr("Space Parity"),QSerialPort::SpaceParity);
    result->addItem(tr("Mark Parity"),QSerialPort::MarkParity);
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    result->addItem(tr("Unknown Parity"),QSerialPort::UnknownParity);
#endif //Only for Qt < 5.15

    return result;
}

QComboBox* SerialPortConfiguratorDialog::_createFlowControlSelector()
{
    QComboBox* result = new QComboBox;

    result->addItem(tr("No Flow control"),QSerialPort::NoFlowControl);
    result->addItem(tr("Hardware Control"),QSerialPort::HardwareControl);
    result->addItem(tr("Software Control"),QSerialPort::SoftwareControl);
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    result->addItem(tr("Unknown Flow Control"),QSerialPort::UnknownFlowControl);
#endif //Only for Qt < 5.15

    return result;
}
