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

    void allowMultipleSelection(bool arg);

    void setCurrentSerialDeviceList(const QList<QSerialPortInfo>& portList);

signals:
    void updateRequested();

    void deviceOpeningRequested(const QSerialPortInfo& serialPortInfo);
    void deviceClosureRequested(const QSerialPortInfo& serialPortInfo);

public slots:
    void clearDeviceList();

    void deviceWasAttached(const QSerialPortInfo& details);
    void deviceWasDetached(const QSerialPortInfo& details);

    void deviceWasOpened(const QSerialPortInfo& details);
    void deviceWasClosed(const QSerialPortInfo& details);

private slots:
    void _deviceActionWasToggled(bool state);

private:
    void           _setupUi();
    QAction*       _findDeviceAction(const QSerialPortInfo& details);
    void           _addDeviceActionEntry(const QSerialPortInfo& details, bool isConnected = false);
    QActionGroup*  w_deviceActionsGroup;
    QAction*       w_separatorAction;
};
Q_DECLARE_METATYPE(QSerialPortInfo);

#endif // SERIALDEVICESELECTORMENU_H
