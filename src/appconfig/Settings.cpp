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

#include "Settings.h"

void Settings::applyCommandLineParameters(const CommandLineParser& parser)
{
    if (parser.preserveConfig())
        setPreserveConfig(true);

    if (!parser.commandsFile().isEmpty())
        setOpenedCommandsFileName(parser.commandsFile());

#ifdef HID
    if (parser.inputDeviceFilterConfigured())
        appendInputDeviceFilter(parser.inputDeviceFilter());
#endif //HID

#ifdef SERIAL
    if (parser.serialDeviceFilterConfigured())
        appendSerialDeviceFilter(parser.serialDeviceFilter());
#endif
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
    RfidControllerSettings::loadValues();

#ifdef GUI
    MainWindowSettings::loadValues();
#endif
}
