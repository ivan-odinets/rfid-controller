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

#include "MainWindowSettings.h"

static const QLatin1String MINIMIZE_ON_CLOSE(    "gui/minimizeOnClose"         );
static const QLatin1String WINDOW_SIZE(          "gui/windowSize"              );
static const QLatin1String RECENT_FILES(         "gui/recentFiles"             );
static const QLatin1String START_HIDDEN(         "gui/startHidden"             );
static const QLatin1String NOTIFY_ATTACHED_DEVS( "gui/notifyAttachedDevices"   );
static const QLatin1String NOTIFY_DETACHED_DEVS( "gui/notifyDetachedDevices"   );
static const QLatin1String NOTIFY_OPENED_DEVS(   "gui/notifyOpenedDevices"     );
static const QLatin1String NOTIFY_CLOSED_DEVS(   "gui/notifyClosedDevices"     );
static const QLatin1String NOTIFY_ERRORS(        "gui/notifyErrors"            );
static const QLatin1String ACTIVE_WIDGET(        "gui/activeWidget"            );

#ifndef QT_NO_SYSTEMTRAYICON
static const QLatin1String MINIMIZE_TO_TRAY(     "gui/minimizeToTray"          );
#endif //QT_NO_SYSTEMTRAYICON

MainWindowSettings* MainWindowSettings::theOne = nullptr;

void MainWindowSettings::_loadValues()
{
    m_windowSize                  = _value(WINDOW_SIZE).toSize();
    m_minimizeOnClose             = _value(MINIMIZE_ON_CLOSE,true).toBool();

    m_notifyAboutAttachedDevices  = (_contains(NOTIFY_ATTACHED_DEVS)) ?
            Notification::typeFromString(_value(NOTIFY_ATTACHED_DEVS).toString())        : Notification::None;

    m_notifyAboutDetachedDevices  = (_contains(NOTIFY_DETACHED_DEVS)) ?
                Notification::typeFromString(_value(NOTIFY_DETACHED_DEVS).toString())    : Notification::None;

    m_notifyAboutOpenedDevices    = (_contains(NOTIFY_OPENED_DEVS)) ?
                Notification::typeFromString(_value(NOTIFY_OPENED_DEVS).toString())      : Notification::None;

    m_notifyAboutClosedDevices    = (_contains(NOTIFY_CLOSED_DEVS)) ?
            Notification::typeFromString(_value(NOTIFY_CLOSED_DEVS).toString())          : Notification::None;

    m_notifyAboutErrors           = (_contains(NOTIFY_ERRORS)) ?
            Notification::typeFromString(_value(NOTIFY_ERRORS).toString())               : Notification::MessageBox;

    m_startHidden                 = _value(START_HIDDEN,false).toBool();
    m_recentFiles                 = _value(RECENT_FILES).toStringList();

#ifdef LOG
    m_activeWidget                = _value(ACTIVE_WIDGET,0).toInt();
#endif //LOG

#ifndef QT_NO_SYSTEMTRAYICON
    m_minimizeToTray              = _value(MINIMIZE_TO_TRAY,true).toBool();
#endif //QT_NO_SYSTEMTRAYICON

#ifdef LOG
    LoggerWidgetSettings::_loadValues();
#endif
}

void MainWindowSettings::setWindowSize(const QSize &windowSize)
{
    m_windowSize = windowSize;
    _setValue(WINDOW_SIZE,windowSize);
}

void MainWindowSettings::setMinimizeOnClose(bool state)
{
    m_minimizeOnClose = state;
    _setValue(MINIMIZE_ON_CLOSE,state);
}

void MainWindowSettings::setNotifyAboutAttachedDevices(Notification::Type newType)
{
    m_notifyAboutAttachedDevices = newType;
    _setValue(NOTIFY_ATTACHED_DEVS,Notification::typeToString(newType));
}

void MainWindowSettings::setNotifyAboutDetachedDevices(Notification::Type newType)
{
    m_notifyAboutDetachedDevices = newType;
    _setValue(NOTIFY_DETACHED_DEVS,Notification::typeToString(newType));
}

void MainWindowSettings::setNotifyAboutOpenedDevices(Notification::Type newType)
{
    m_notifyAboutOpenedDevices = newType;
    _setValue(NOTIFY_OPENED_DEVS,Notification::typeToString(newType));
}

void MainWindowSettings::setNotifyAboutClosedDevices(Notification::Type newType)
{
    m_notifyAboutClosedDevices = newType;
    _setValue(NOTIFY_CLOSED_DEVS,Notification::typeToString(newType));
}

void MainWindowSettings::setNotifyAboutErrors(Notification::Type newType)
{
    m_notifyAboutErrors = newType;
    _setValue(NOTIFY_ERRORS,Notification::typeToString(newType));
}

void MainWindowSettings::setStartHidden(bool state)
{
    m_startHidden = state;
    _setValue(START_HIDDEN,state);
}

void MainWindowSettings::setRecentFiles(const QStringList& recentFiles)
{
    m_recentFiles = recentFiles;
    _setValue(RECENT_FILES,QVariant(recentFiles));
}

#ifdef LOG
//
// This part is needed only if we have Logging support enabled
//

void MainWindowSettings::setActiveWidget(int index)
{
    m_activeWidget = index;
    _setValue(ACTIVE_WIDGET,index);
}

#endif //LOG

#ifndef QT_NO_SYSTEMTRAYICON
//
// This part is needed only if we have SystemTray support enabled
//

void MainWindowSettings::setMinimizeToTray(bool state)
{
    _setValue(MINIMIZE_TO_TRAY,state);
}

#endif //QT_NO_SYSTEMTRAYICON
