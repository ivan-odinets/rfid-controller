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

#ifndef SERIALDEVICESELECTORMENU_H
#define SERIALDEVICESELECTORMENU_H

#include <QMenu>

#include <QSerialPortInfo>

class SerialDeviceSelectorMenu : public QMenu
{
    Q_OBJECT
public:
    explicit SerialDeviceSelectorMenu(QWidget *parent = nullptr);
    explicit SerialDeviceSelectorMenu(const QString &title, QWidget *parent = nullptr);
    ~SerialDeviceSelectorMenu();

    /*! @brief Is selection of multiple Serial devices is allowed. By default is set to false. */
    void allowMultipleSelection(bool arg);

    /*! @brief Display list of Serial devices, as attached but not opened. */
    void setCurrentSerialDeviceList(const QList<QSerialPortInfo>& portList);

signals:
    /*! @brief This signal is emitted when user asks to manually update list of serial devices. */
    void updateRequested();

    /*! @brief This signal is emitted when user asks to open serial device represented by QSerialPortInfo object. */
    void deviceOpeningRequested(const QSerialPortInfo& serialPortInfo);

    /*! @brief This signal is emitted when user asks to close serial device represented by QSerialPortInfo object. */
    void deviceClosureRequested(const QSerialPortInfo& serialPortInfo);

public slots:
    /*! @brief This slot can be used to clear Menu from devices. No signals are emitted while executing this slot. */
    void clearDeviceList();

    /*! @brief This slot should be invoked when new serial device was attached. */
    void deviceWasAttached(const QSerialPortInfo& details);

    /*! @brief This slot should be invoked when new serial device was detached. */
    void deviceWasDetached(const QSerialPortInfo& details);

    /*! @brief This slot should be invoked when new serial device was opened. */
    void deviceWasOpened(const QSerialPortInfo& details);

    /*! @brief This slot should be invoked when new serial device was closed. */
    void deviceWasClosed(const QSerialPortInfo& details);

private slots:
    void _deviceActionWasToggled(bool state);

private:
    inline void    _setupUi();
    QAction*       _findDeviceAction(const QSerialPortInfo& details);
    void           _addDeviceActionEntry(const QSerialPortInfo& details, bool isConnected = false);
    QActionGroup*  w_deviceActionsGroup;
    QAction*       w_separatorAction;
};
Q_DECLARE_METATYPE(QSerialPortInfo);

#endif // SERIALDEVICESELECTORMENU_H
