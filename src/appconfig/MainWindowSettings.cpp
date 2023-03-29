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

#include "MainWindowSettings.h"

#define MINIMIZE_ON_CLOSE         QStringLiteral("gui/minimizeOnClose")
#define WINDOW_SIZE               QStringLiteral("gui/windowSize")
#define RECENT_FILES              QStringLiteral("gui/recentFiles")
#define START_HIDDEN              QStringLiteral("gui/startHidden")
#define NOTIFY_ATTACHED_DEVS      QStringLiteral("gui/notifyAttachedDevices")
#define NOTIFY_DETACHED_DEVS      QStringLiteral("gui/notifyDetachedDevices")
#define NOTIFY_OPENED_DEVS        QStringLiteral("gui/notifyOpenedDevices")
#define NOTIFY_CLOSED_DEVS        QStringLiteral("gui/notifyClosedDevices")
#define NOTIFY_ERRORS             QStringLiteral("gui/notifyErrors")

#ifndef QT_NO_SYSTEMTRAYICON
    #define MINIMIZE_TO_TRAY      QStringLiteral("gui/minimizeToTray")
#endif //QT_NO_SYSTEMTRAYICON

MainWindowSettings* MainWindowSettings::theOne = nullptr;

void MainWindowSettings::loadValues()
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

#ifndef QT_NO_SYSTEMTRAYICON
    m_minimizeToTray              = _value(MINIMIZE_TO_TRAY,true).toBool();
#endif //QT_NO_SYSTEMTRAYICON
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

#ifndef QT_NO_SYSTEMTRAYICON
//
// This part is needed only if we have SystemTray support enabled
//

void MainWindowSettings::setMinimizeToTray(bool state)
{
    _setValue(MINIMIZE_TO_TRAY,state);
}

#endif //QT_NO_SYSTEMTRAYICON
