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

#include "CommandLineParser.h"

CommandLineParser::CommandLineParser() :
    m_configFile(       QStringList{ "c", "config"}           ),
    m_preserveConfig(   QStringList{ "p", "preserve-config" } ),
    m_commandsFile(     QStringList{ "r", "commands" }        )
#ifdef GUI
    ,m_noGui(      "no-gui"       ),
    m_startHidden( "start-hidden" )
#endif //GUI

#ifdef LOG
    ,m_logFile(         QStringList{ "l", "log"}              )
#endif

#ifdef HID
    ,m_inputVendorIds(       "hid-vendors"  ),
    m_inputProductIds(       "hid-products" )
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    ,m_inputDeviceFileNames( "hid-names"    )
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC

#endif //HID

#ifdef SERIAL
    ,m_serialVendorIds(  "tty-vendors"   ),
    m_serialProductIds(  "tty-products"   ),
    m_serialDeviceNames( "tty-names"  )
#endif //SERIAL
{
    // -h / --help
    addHelpOption();

    // -v / --version
    addVersionOption();

    // -c / --config
    m_configFile.setValueName("file");
    m_configFile.setDescription(tr("Specify config <file> to use."));
    addOption(m_configFile);

    // -p / --preserve-config
    m_preserveConfig.setDescription(tr("Do not change configuration file."));
    addOption(m_preserveConfig);

    // -r / --commands
    m_commandsFile.setValueName("file");
    m_commandsFile.setDescription(tr("Commands <file> to open."));
    addOption(m_commandsFile);

    //
    // This part is needed only if we have GUI support enabled
    //
#ifdef GUI

    // --no-gui
    m_noGui.setDescription(tr("Do not create any GUI elements."));
    addOption(m_noGui);

    // --start-hidden
    m_startHidden.setDescription(tr("Start application with hidden main window."));
    addOption(m_startHidden);
#endif //GUI

#ifdef LOG
    m_logFile.setDescription(tr("Speciyf log <file> to use."));
    addOption(m_logFile);
#endif

#ifdef HID
    //
    // This part is needed only if we have GUI support enabled
    //

    // / --hid-vids
    m_inputVendorIds.setValueName("list");
    m_inputVendorIds.setDescription(tr("List of comma-separated HID VendorIds, which should be connected automaticaly."));
    addOption(m_inputVendorIds);

    // / --hid-pids
    m_inputProductIds.setValueName("list");
    m_inputProductIds.setDescription(tr("List of comma-separated HID ProductIds, which should be connected automaticaly."));
    addOption(m_inputProductIds);

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    //
    // Linux-specific code
    //

    // / --hid-names
    m_inputDeviceFileNames.setValueName("list");
    m_inputDeviceFileNames.setDescription("List of comma-separated /dev/input/* files, which should be connected automaticaly.");
    addOption(m_inputDeviceFileNames);

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

    // / --tty-vendors
    m_serialVendorIds.setValueName("list");
    m_serialVendorIds.setDescription(tr("List of comma-separated Serial VendorIds, which should be connected automaticaly."));
    addOption(m_serialVendorIds);

    // / --tty-products
    m_serialProductIds.setValueName("list");
    m_serialProductIds.setDescription(tr("List of comma-separated Serial ProductIds, which should be connected automaticaly."));
    addOption(m_serialProductIds);

    // / --tty-names
    m_serialDeviceNames.setValueName("list");
    m_serialDeviceNames.setDescription(tr("List of comma-separated Serial Port Names, which should be connected automaticaly.")),
    addOption(m_serialDeviceNames);
#endif //SERIAL
}

/*
 **********************************************************************************************************************
 * This part is needed only if we have HID support enabled
 */

#ifdef HID

bool CommandLineParser::inputDeviceFilterConfigured() const
{
    return (isSet(m_inputDeviceFileNames))
            || ( isSet(m_inputProductIds) && isSet(m_inputVendorIds) );
}

InputDeviceFilter CommandLineParser::inputDeviceFilter() const
{
    InputDeviceFilter result;

    result.setVendorIdSet(inputVendorIdsSet());
    result.setProductIdSet(inputProductIdsSet());
    result.setDeviceNameSet(inputDeviceFileNames());

    return result;
}

VendorIdSet CommandLineParser::inputVendorIdsSet() const
{
    return VendorIdSet(value(m_inputVendorIds));
}

ProductIdSet CommandLineParser::inputProductIdsSet() const
{
    return ProductIdSet(value(m_inputProductIds));
}

DeviceNameSet CommandLineParser::inputDeviceFileNames() const
{
    return DeviceNameSet(value(m_inputDeviceFileNames));
}

#endif //HID

/*
 **********************************************************************************************************************
 * This part is needed only if we have Serial support enabled
 */

#ifdef SERIAL

bool CommandLineParser::serialDeviceFilterConfigured() const
{
    return (isSet(m_serialDeviceNames))
            || ( isSet(m_serialProductIds) && isSet(m_serialVendorIds) );
}

SerialPortFilter CommandLineParser::serialDeviceFilter() const
{
    SerialPortFilter result;

    result.setVendorIdsSet(serialVendorIdsSet());
    result.setProductIdsSet(serialProductIdsSet());
    result.setPortNameSet(serialDeviceNamesSet());

    return result;
}

VendorIdSet CommandLineParser::serialVendorIdsSet() const
{
    return VendorIdSet(value(m_serialVendorIds));
}

ProductIdSet CommandLineParser::serialProductIdsSet() const
{
    return ProductIdSet(value(m_serialProductIds));
}

DeviceNameSet CommandLineParser::serialDeviceNamesSet() const
{
    return DeviceNameSet(value(m_serialDeviceNames));
}

#endif //SERIAL
