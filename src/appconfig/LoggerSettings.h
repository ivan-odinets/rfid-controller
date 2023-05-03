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

#ifndef LOGGERSETTINGS_H
#define LOGGERSETTINGS_H

#include "SettingsCore.h"

class LoggerSettings : public virtual SettingsCore
{
public:
    static LoggerSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    QString   logFile() const               { return m_logFile; }
    void      setLogFile(const QString& logFile);

    bool      logKeys() const               { return m_logKeys; }
    void      setKeysLogging(bool state);

    bool      logMatchedKeys() const        { return m_logMatchedkeys; }
    void      setMatchedKeysLogging(bool state);

    bool      logErrors() const             { return m_logErrors; }
    void      setLoggingErrors(bool state);

    bool      logAttachedDevices() const    { return m_logAttachedDevices; }
    void      setAttachedDeviceLogging(bool state);

    bool      logDetachedDevices() const    { return m_logDetachedDevices; }
    void      setDetachedDeviceLogging(bool state);

    bool      logOpenedDevices() const      { return m_logOpenedDevices; }
    void      setOpenedDevicesLogging(bool state);

    bool      logClosedDevices() const      { return m_logClosedDevices; }
    void      setClosedDeviceLogging(bool state);

protected:
    LoggerSettings() {
        //Save this, so some other code parts can access only this specific part of settings
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }
    void _loadValues();

private:
    static LoggerSettings* theOne;

    QString   m_logFile;

    bool m_logKeys;
    bool m_logMatchedkeys;

    bool m_logErrors;

    bool m_logAttachedDevices;
    bool m_logDetachedDevices;

    bool m_logOpenedDevices;
    bool m_logClosedDevices;
};
#define loggerSettings LoggerSettings::get()

#endif // LOGGERSETTINGS_H
