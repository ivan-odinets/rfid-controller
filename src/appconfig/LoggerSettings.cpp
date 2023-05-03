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

#include "LoggerSettings.h"

LoggerSettings* LoggerSettings::theOne = nullptr;

static const QLatin1String LOG_FILE              ("log/file"              );

static const QLatin1String LOG_KEYS(             "log/keys"               );
static const QLatin1String LOG_MATCHED_KEYS(     "log/matchedKeys"        );
static const QLatin1String LOG_ERRORS(           "log/errors"             );

static const QLatin1String LOG_ATTACHED(         "log/attachedDevices"    );
static const QLatin1String LOG_DETACHED(         "log/detachedDevices"    );

static const QLatin1String LOG_OPENED(           "log/openedDevices"      );
static const QLatin1String LOG_CLOSED(           "log/closedDevices"      );

void LoggerSettings::_loadValues()
{
    m_logFile = _value(LOG_FILE).toString();

    m_logKeys = _value(LOG_KEYS).toBool();
    m_logMatchedkeys = _value(LOG_MATCHED_KEYS).toBool();

    m_logErrors = _value(LOG_ERRORS).toBool();

    m_logOpenedDevices = _value(LOG_OPENED).toBool();
    m_logClosedDevices = _value(LOG_CLOSED).toBool();

    m_logAttachedDevices = _value(LOG_ATTACHED).toBool();
    m_logDetachedDevices = _value(LOG_DETACHED).toBool();
}

void LoggerSettings::setLogFile(const QString& logFile)
{
    m_logFile = logFile;
    _setValue(LOG_FILE,logFile);
}

void LoggerSettings::setKeysLogging(bool state)
{
    m_logKeys = state;
    _setValue(LOG_KEYS,state);
}

void LoggerSettings::setMatchedKeysLogging(bool state)
{
    m_logMatchedkeys = state;
    _setValue(LOG_MATCHED_KEYS,state);
}

void LoggerSettings::setLoggingErrors(bool state)
{
    m_logErrors = state;
    _setValue(LOG_ERRORS,state);
}

void LoggerSettings::setAttachedDeviceLogging(bool state)
{
    m_logAttachedDevices = state;
    _setValue(LOG_ATTACHED,state);
}

void LoggerSettings::setDetachedDeviceLogging(bool state)
{
    m_logDetachedDevices = state;
    _setValue(LOG_DETACHED,state);
}

void LoggerSettings::setOpenedDevicesLogging(bool state)
{
    m_logOpenedDevices = state;
    _setValue(LOG_OPENED,state);
}

void LoggerSettings::setClosedDeviceLogging(bool state)
{
   m_logClosedDevices = state;
   _setValue(LOG_CLOSED,state);
}
