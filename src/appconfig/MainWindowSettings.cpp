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
#define NOTIFY_OPENED_DEVS        QStringLiteral("gui/notifyOpenedDevices")

#ifndef QT_NO_SYSTEMTRAYICON
    #define MINIMIZE_TO_TRAY      QStringLiteral("gui/minimizeToTray")
#endif //QT_NO_SYSTEMTRAYICON

MainWindowSettings* MainWindowSettings::theOne = nullptr;

void MainWindowSettings::loadValues()
{
    m_windowSize = _value(WINDOW_SIZE).toSize();
    m_minimizeOnClose = _value(MINIMIZE_ON_CLOSE).toBool();
    m_notifyAboutOpenedDevices = _value(NOTIFY_OPENED_DEVS).toString();
    m_startHidden = _value(START_HIDDEN).toBool();
    m_recentFiles = _value(RECENT_FILES).toStringList();

#ifndef QT_NO_SYSTEMTRAYICON
    m_minimizeToTray = _value(MINIMIZE_TO_TRAY).toBool();
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

void MainWindowSettings::setNotifyAboutOpenedDevices(const QString& value)
{
    m_notifyAboutOpenedDevices = value;
    _setValue(NOTIFY_OPENED_DEVS,value);
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
