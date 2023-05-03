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

#include "LoggingMenu.h"

LoggingMenu::LoggingMenu(QWidget* parent) :
    QMenu{parent}
{
    _setupUi();
}

LoggingMenu::LoggingMenu(const QString& title, QWidget* parent) :
    QMenu{title,parent}
{
    _setupUi();
}

void LoggingMenu::_setupUi()
{
    w_logDiscoveredKeysAction = this->addAction(tr("Keys discovered"));
    w_logDiscoveredKeysAction->setCheckable(true);
    connect(w_logDiscoveredKeysAction,&QAction::triggered,this,&LoggingMenu::logDiscoveredKeysTriggered);

    w_logMatchedKeysAction = this->addAction(tr("Keys matched"));
    w_logMatchedKeysAction->setCheckable(true);
    connect(w_logMatchedKeysAction,&QAction::triggered,this,&LoggingMenu::logMatchedKeysTriggered);

    w_logAttachedDevicesAction = this->addAction(tr("Attached devices"));
    w_logAttachedDevicesAction->setCheckable(true);
    connect(w_logAttachedDevicesAction,&QAction::triggered,this,&LoggingMenu::logAttachedDevicesTriggered);

    w_logDetachedDevicesAction = this->addAction(tr("Detached devices"));
    w_logDetachedDevicesAction->setCheckable(true);
    connect(w_logDetachedDevicesAction,&QAction::triggered,this,&LoggingMenu::logDetachedDevicesTriggered);

    w_logOpenedDevicesAction = this->addAction(tr("Opened devices"));
    w_logOpenedDevicesAction->setCheckable(true);
    connect(w_logOpenedDevicesAction,&QAction::triggered,this,&LoggingMenu::logOpenedDevicesTriggered);

    w_logClosedDevicesAction = this->addAction(tr("Closed devices"));
    w_logClosedDevicesAction->setCheckable(true);
    connect(w_logClosedDevicesAction,&QAction::triggered,this,&LoggingMenu::logClosedDevicesTriggered);

    w_logErrorsAction = this->addAction(tr("Errors logging"));
    w_logErrorsAction->setCheckable(true);
    connect(w_logErrorsAction,&QAction::triggered,this,&LoggingMenu::logErrorsTriggered);
}
