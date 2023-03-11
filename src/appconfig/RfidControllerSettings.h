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

#ifndef RFIDCONTROLLERSETTINGS_H
#define RFIDCONTROLLERSETTINGS_H

#include "./SettingsCore.h"

#ifdef HID
    #include "./InputDeviceManagerSettings.h"
#endif //HID

#ifdef SERIAL
    #include "./SerialDeviceManagerSettings.h"
#endif //SERIAL

class RfidControllerSettings : public virtual SettingsCore
#ifdef HID
    ,public virtual InputDeviceManagerSettings
#endif //HID
#ifdef SERIAL
    ,public virtual SerialDeviceManagerSettings
#endif //SERIAL
{
public:
    static RfidControllerSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    void      loadValues();

    QString   openedCommandsFileName() const { return m_openedCommandsFileName; }
    void      setOpenedCommandsFileName(const QString& fileName);

protected:
    RfidControllerSettings() {
        //Save this, so some other code parts can access only this specific part of settings
        Q_ASSERT(theOne == nullptr);
        theOne = this;
    }

private:
    Q_DISABLE_COPY(RfidControllerSettings)
    static RfidControllerSettings* theOne;

    QString m_openedCommandsFileName;
};

#endif // RFIDCONTROLLERSETTINGS_H
