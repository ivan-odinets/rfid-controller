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

#ifndef LOGGERWIDGETSETTINGS_H
#define LOGGERWIDGETSETTINGS_H

#include "SettingsCore.h"

class LoggerWidgetSettings : public virtual SettingsCore
{
public:
    static LoggerWidgetSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    bool logToWidget() const { return m_logToWidget; }
    void setLogToWidget(bool state);

protected:
    LoggerWidgetSettings() {
        //Save this, so some other code parts can access only this specific part of settings
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }
    void _loadValues();

private:
    static LoggerWidgetSettings* theOne;

    bool m_logToWidget;
};
#define loggerWidgetSettings LoggerWidgetSettings::get()

#endif // LOGGERWIDGETSETTINGS_H
