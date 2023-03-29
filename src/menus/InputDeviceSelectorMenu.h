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

#ifndef INPUTDEVICESELECTORMENU_H
#define INPUTDEVICESELECTORMENU_H

#include <QMenu>

#include "core/input/InputDeviceInfo.h"

class InputDeviceSelectorMenu : public QMenu
{
    Q_OBJECT
public:
    explicit InputDeviceSelectorMenu(QWidget *parent = nullptr);
    explicit InputDeviceSelectorMenu(const QString &title, QWidget *parent = nullptr);
    ~InputDeviceSelectorMenu();

    void allowMultipleSelection(bool arg);

    void setCurrentInputDeviceList(const InputDeviceInfoList& deviceList);

signals:
    void updateRequested();

    void deviceOpeningRequested(const InputDeviceInfo& deviceDetails);
    void deviceClosureRequested(const InputDeviceInfo& deviceDetails);

public slots:
    void clearDeviceList();

    void deviceWasAttached(const InputDeviceInfo& details);
    void deviceWasDetached(const InputDeviceInfo& details);

    void deviceWasOpened(const InputDeviceInfo& details);
    void deviceWasClosed(const InputDeviceInfo& details);

private slots:
    void _deviceActionWasToggled(bool state);

private:
    void           _setupUi();
    QAction*       _findDeviceAction(const InputDeviceInfo& details);
    void           _addDeviceActionEntry(const InputDeviceInfo& details, bool isConnected = false);
    QActionGroup*  w_deviceActionsGroup;
    QAction*       w_separatorAction;
};

#endif // INPUTDEVICESELECTORMENU_H
