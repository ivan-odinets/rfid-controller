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

#ifndef MAINWINDOWSETTINGS_H
#define MAINWINDOWSETTINGS_H

#ifdef LOG
    #include "LoggerWidgetSettings.h"
#else
    #include "SettingsCore.h"
#endif

#include <QSize>

#include "core/NotificationType.h"

#ifdef LOG
class MainWindowSettings : public virtual LoggerWidgetSettings
#else
class MainWindowSettings : public virtual SettingsCore
#endif
{
public:
    static MainWindowSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }
    virtual ~MainWindowSettings() {};

    QSize  windowSize() const                    { return m_windowSize; }
    void   setWindowSize(const QSize& windowSize);

    bool   minimizeOnClose() const               { return m_minimizeOnClose; }
    void   setMinimizeOnClose(bool state);

    Notification::Type  notifyAboutAttachedDevices() const { return m_notifyAboutAttachedDevices; }
    void                setNotifyAboutAttachedDevices(Notification::Type newType);

    Notification::Type  notifyAboutDetachedDevices() const { return m_notifyAboutDetachedDevices; }
    void                setNotifyAboutDetachedDevices(Notification::Type newType);

    Notification::Type  notifyAboutOpenedDevices() const   { return m_notifyAboutOpenedDevices; }
    void                setNotifyAboutOpenedDevices(Notification::Type newType);

    Notification::Type  notifyAboutClosedDevices() const   { return m_notifyAboutClosedDevices; }
    void                setNotifyAboutClosedDevices(Notification::Type newType);

    Notification::Type  notifyAboutErrors() const          { return m_notifyAboutErrors; }
    void                setNotifyAboutErrors(Notification::Type newType);

    bool startHidden() const                     { return m_startHidden; }
    void setStartHidden(bool state);

    QStringList    recentFiles() const           { return m_recentFiles; }
    void           setRecentFiles(const QStringList& recent);

protected:
    MainWindowSettings() {
        //Save this, so some other code parts can access only this specific part of settings
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }
    void _loadValues();

private:
    static MainWindowSettings* theOne;

    QSize               m_windowSize;
    bool                m_minimizeOnClose;
    Notification::Type  m_notifyAboutAttachedDevices;
    Notification::Type  m_notifyAboutDetachedDevices;
    Notification::Type  m_notifyAboutOpenedDevices;
    Notification::Type  m_notifyAboutClosedDevices;
    Notification::Type  m_notifyAboutErrors;

    bool                m_startHidden;
    QStringList         m_recentFiles;

#ifdef LOG
//
// This part is needed only if we have Logging support enabled
//

public:
    int activeWidget() const                     { return m_activeWidget; }
    void setActiveWidget(int index);

private:
    int                 m_activeWidget;

#endif //LOG

#ifndef QT_NO_SYSTEMTRAYICON
//
// This part is needed only if we have SystemTray support enabled
//

public:
    bool   minimizeToTray() const      { return m_minimizeToTray; }
    void   setMinimizeToTray(bool state);

private:
    bool   m_minimizeToTray;
#endif //QT_NO_SYSTEMTRAYICON
};
#define mainWindowSettings MainWindowSettings::get()

#endif // MAINWINDOWSETTINGS_H
