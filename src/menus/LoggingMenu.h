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

#ifndef LOGGINGMENU_H
#define LOGGINGMENU_H

#include <QAction>
#include <QMenu>

/*!
 *  @class LoggingMenu menus/LoggingMenu.h
 *  @brief This class allows creation of menus, which allow user to select what events will be placed to log file.
 */

class LoggingMenu : public QMenu
{
    Q_OBJECT
public:
    explicit LoggingMenu(QWidget* parent = nullptr);
    explicit LoggingMenu(const QString& title, QWidget* parent = nullptr);
    ~LoggingMenu() {}

signals:
    void enableLoggingTriggered(bool state);

    void logDiscoveredKeysTriggered(bool state);
    void logMatchedKeysTriggered(bool state);

    void logAttachedDevicesTriggered(bool state);
    void logDetachedDevicesTriggered(bool state);

    void logOpenedDevicesTriggered(bool state);
    void logClosedDevicesTriggered(bool state);

    void logErrorsTriggered(bool state);

public slots:
    void setLogDiscoveredKeysChecked(bool state)      { w_logDiscoveredKeysAction->setChecked(state); }
    void setLogMatchedKeysChecked(bool state)         { w_logMatchedKeysAction->setChecked(state); }

    void setLogAttachedDevicesChecked(bool state)     { w_logAttachedDevicesAction->setChecked(state); }
    void setLogDetachedDevicesChecked(bool state)     { w_logDetachedDevicesAction->setChecked(state); }

    void setLogOpenedDevicesChecked(bool state)       { w_logOpenedDevicesAction->setChecked(state); }
    void setLogClosedDevicesChecked(bool state)       { w_logClosedDevicesAction->setChecked(state); }

    void setLogErrorsChecked(bool state)              { w_logErrorsAction->setChecked(state); }

private:
    inline void _setupUi();

    QAction* w_logDiscoveredKeysAction;
    QAction* w_logMatchedKeysAction;

    QAction* w_logAttachedDevicesAction;
    QAction* w_logDetachedDevicesAction;

    QAction* w_logOpenedDevicesAction;
    QAction* w_logClosedDevicesAction;

    QAction* w_logErrorsAction;
};

#endif // LOGGINGMENU_H
