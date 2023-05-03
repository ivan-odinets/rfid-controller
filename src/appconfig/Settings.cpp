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

#include "Settings.h"

void Settings::applyCommandLineParameters(const CommandLineParser& parser)
{
    if (parser.preserveConfig())
        setPreserveConfig(true);

    if (!parser.commandsFile().isEmpty())
        setOpenedCommandsFileName(parser.commandsFile());

#ifdef LOG
    if (!parser.logFile().isEmpty())
        setLogFile(parser.logFile());
#endif //LOG

#ifdef HID
    if (parser.inputDeviceFilterConfigured())
        appendInputDeviceFilter(parser.inputDeviceFilter());
#endif //HID

#ifdef SERIAL
    if (parser.serialDeviceFilterConfigured())
        appendSerialDeviceFilter(parser.serialDeviceFilter());
#endif //SERIAL
}

bool Settings::configParsed()
{
    if (!SettingsCore::_initialized())
        return false;

    loadValues();
    return true;
}

void Settings::loadValues()
{
    RfidControllerSettings::_loadValues();

#ifdef GUI
    MainWindowSettings::_loadValues();
#endif
}
