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

#ifndef MAINWINDOWSETTINGS_H
#define MAINWINDOWSETTINGS_H

#include "SettingsCore.h"

#include <QSize>

#include "core/NotificationType.h"

class MainWindowSettings : public virtual SettingsCore
{
public:
    static MainWindowSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }
    virtual ~MainWindowSettings() {};

    void loadValues();

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

#endif // MAINWINDOWSETTINGS_H
