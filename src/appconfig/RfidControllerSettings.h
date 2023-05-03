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

#ifndef RFIDCONTROLLERSETTINGS_H
#define RFIDCONTROLLERSETTINGS_H

#include "./SettingsCore.h"

#ifdef HID
    #include "./InputDeviceManagerSettings.h"
#endif //HID

#ifdef SERIAL
    #include "./SerialDeviceManagerSettings.h"
#endif //SERIAL

#ifdef LOG
    #include "./LoggerSettings.h"
#endif

class RfidControllerSettings : public virtual SettingsCore
#ifdef HID
    ,public virtual InputDeviceManagerSettings
#endif //HID
#ifdef SERIAL
    ,public virtual SerialDeviceManagerSettings
#endif //SERIAL
#ifdef LOG
    ,public virtual LoggerSettings
#endif //LOG
{
public:
    static RfidControllerSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    QString   openedCommandsFileName() const { return m_openedCommandsFileName; }
    void      setOpenedCommandsFileName(const QString& fileName);

protected:
    RfidControllerSettings() {
        //Save this, so some other code parts can access only this specific part of settings
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }
    void      _loadValues();

private:
    Q_DISABLE_COPY(RfidControllerSettings)
    static RfidControllerSettings* theOne;

    QString m_openedCommandsFileName;
};

#endif // RFIDCONTROLLERSETTINGS_H
