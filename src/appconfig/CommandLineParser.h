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

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>

#ifdef HID
    #include "core/input/InputDeviceFilter.h"
#endif //HID

#ifdef SERIAL
    #include "core/serial/SerialPortFilter.h"
#endif //SERIAL

/*!
 * @class CommandLineParser appconfig/CommandLineParser.h
 * @brief This class is responsible for parsing command line options.
 * @details This class is just a subclass of QCommandLineParser. Important thing - by default, command line options
 *          overrides options configured in *.config file with updating config. Only if preserve-config option is
 *          specified - *.config file will stay unchanged during the whole application run.
 */

class CommandLineParser : public QCommandLineParser
{
    Q_DECLARE_TR_FUNCTIONS(CommandLineParser);
public:
    CommandLineParser();
    ~CommandLineParser() {}

    QString             configFile() const       { return value(m_configFile); }
    bool                preserveConfig() const   { return isSet(m_preserveConfig); }

    QString             commandsFile() const     { return value(m_commandsFile); }

private:
    QCommandLineOption m_configFile;
    QCommandLineOption m_preserveConfig;

    QCommandLineOption m_commandsFile;

#ifdef GUI
//
// This part is needed only if we have GUI support enabled
//

public:
    bool noGui() const                           { return isSet(m_noGui); }
    bool startHidden() const                     { return isSet(m_startHidden); }

private:
    QCommandLineOption  m_noGui;
    QCommandLineOption  m_startHidden;
#endif //GUI

#ifdef LOG
//
// This part is needed only if we have log support enabled
//

public:
    QString logFile() const                      { return value(m_logFile); }

private:
    QCommandLineOption m_logFile;
#endif

#ifdef HID
//
// This part is needed only if we have HID support enabled
//

public:
    bool                inputDeviceFilterConfigured() const;
    InputDeviceFilter   inputDeviceFilter() const;

    VendorIdSet         inputVendorIdsSet() const;
    ProductIdSet        inputProductIdsSet() const;

private:
    QCommandLineOption  m_inputVendorIds;
    QCommandLineOption  m_inputProductIds;

#if defined(Q_OS_LINUX) && !defined (Q_OS_ANDROID)
//
// Linux-specific part of CommandLineParser
//

public:
    DeviceNameSet       inputDeviceFileNames() const;

private:
    QCommandLineOption  m_inputDeviceFileNames;

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
#endif //HID

#ifdef SERIAL
//
// This part is needed only if we have Serial support enabled
//

public:
    //Serial section
    bool                serialDeviceFilterConfigured() const;
    SerialPortFilter    serialDeviceFilter() const;

    VendorIdSet         serialVendorIdsSet() const;
    ProductIdSet        serialProductIdsSet() const;
    DeviceNameSet       serialDeviceNamesSet() const;

private:
    QCommandLineOption  m_serialVendorIds;
    QCommandLineOption  m_serialProductIds;
    QCommandLineOption  m_serialDeviceNames;
#endif //SERIAL
};

#endif // COMMANDLINEPARSER_H
