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

#ifndef SERIALPORTCONFIGURATORDIALOG_H
#define SERIALPORTCONFIGURATORDIALOG_H

#include <QDialog>
#include <QWidget>

class QComboBox;

#include "core/serial/SerialPortConfig.h"

/*!
 *  @class SerialPortConfiguratorDialog windows/SerialPortConfiguratorDialog.h
 *  @brief This dialog allows user to configure objects of SerialPortConfig type.
 */

class SerialPortConfiguratorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SerialPortConfiguratorDialog(QWidget* parent = nullptr);
    ~SerialPortConfiguratorDialog() {}

    void                          displaySerialPortConfig(const SerialPortConfig& config);
    SerialPortConfig              serialPortConfig() const;

    QSerialPort::BaudRate         baudRate() const;
    QSerialPort::StopBits         stopBits() const;
    QSerialPort::DataBits         dataBits() const;
    QSerialPort::Parity           parity() const;
    QSerialPort::FlowControl      flowControl() const;

    void setBaudRate(QSerialPort::BaudRate rate);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setFlowControl(QSerialPort::FlowControl flowControl);

private:
    inline void         _setupUi();
    inline QComboBox*   _createBaudSelector();
    QComboBox*          w_baudSelector;

    inline QComboBox*   _createStopBitsSelector();
    QComboBox*          w_stopBitsSelector;

    inline QComboBox*   _createDataBitsSelector();
    QComboBox*          w_dataBitsSelector;

    inline QComboBox*   _createParitySelector();
    QComboBox*          w_paritySelector;

    inline QComboBox*   _createFlowControlSelector();
    QComboBox*          w_flowControlSelector;

    template<class C>
    inline void         _selectValue(QComboBox* box,C value);
};

#endif // SERIALPORTCONFIGURATORDIALOG_H
