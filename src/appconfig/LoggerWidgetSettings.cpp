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

#include "LoggerWidgetSettings.h"

LoggerWidgetSettings* LoggerWidgetSettings::theOne = nullptr;

static const QLatin1String LOG_TO_WIDGET(   "logWidget/logToWidget"  );

void LoggerWidgetSettings::_loadValues()
{
    m_logToWidget = _value(LOG_TO_WIDGET).toBool();
}

void LoggerWidgetSettings::setLogToWidget(bool state)
{
    m_logToWidget = state;
    _setValue(LOG_TO_WIDGET,state);
}
