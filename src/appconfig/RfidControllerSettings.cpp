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

#include "RfidControllerSettings.h"

static const QLatin1String OPENED_FILE(     "openedCommandsFile"     );

RfidControllerSettings* RfidControllerSettings::theOne = nullptr;

void RfidControllerSettings::_loadValues()
{
    m_openedCommandsFileName = _value(OPENED_FILE).toString();

#ifdef LOG
    LoggerSettings::_loadValues();
#endif //LOG

#ifdef HID
    InputDeviceManagerSettings::_loadValues();
#endif //HID

#ifdef SERIAL
    SerialDeviceManagerSettings::_loadValues();
#endif //SERIAL
}

void RfidControllerSettings::setOpenedCommandsFileName(const QString &fileName)
{
    m_openedCommandsFileName = fileName;
    _setValue(OPENED_FILE,fileName);
}
