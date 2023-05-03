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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "./CommandLineParser.h"
#include "./RfidControllerSettings.h"

#ifdef GUI
    #include "./MainWindowSettings.h"
#endif //GUI

class Settings : public virtual RfidControllerSettings
#ifdef GUI
    ,public virtual MainWindowSettings
#endif //GUI
{
public:
    static Settings* getSettings() {
        static Settings theOne;
        return &theOne;
    }

    void applyCommandLineParameters(const CommandLineParser& parser);

    bool configParsed();
    void loadValues();

private:
    Settings() {}
    virtual ~Settings() {}
    Q_DISABLE_COPY(Settings);
};

#endif // SETTINGS_H
