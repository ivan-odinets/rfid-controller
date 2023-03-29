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

#include "InputDeviceWatcher.h"

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    #include <QTimer>
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#elif
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific code
//

InputDeviceWatcher::InputDeviceWatcher(QObject* parent)
    : QObject(parent)
{
    m_fileSystemWatcher.addPath(InputDeviceInfo::inputDeviceDirectory().absolutePath());
    m_lastScanResults = InputDeviceInfo::availableInputDevices();
}

void InputDeviceWatcher::start()
{
    connect(&m_fileSystemWatcher,&QFileSystemWatcher::directoryChanged,this,&InputDeviceWatcher::updateDeviceList);
}

void InputDeviceWatcher::stop()
{
    disconnect(&m_fileSystemWatcher,&QFileSystemWatcher::directoryChanged,this,&InputDeviceWatcher::updateDeviceList);
}

void InputDeviceWatcher::updateDeviceList()
{
    //Function can be called either manually or by signal from QFileSystemWatcher::directoryChanged
    //This signal is not providing info about new/deleted files, so we need to do it manualy
    //by comparing new entries in /dev/input with old results

    InputDeviceInfoList newDeviceInfoList = InputDeviceInfo::availableInputDevices();

    //Search for attached devices
    for (int i = 0; i < newDeviceInfoList.count(); i++) {
        if (!m_lastScanResults.contains(newDeviceInfoList.at(i))) {
            //Small delay needed so that systemd can change permissions (if needed) of device file to allow read acces...
            QTimer::singleShot(500,[this,newDeviceInfoList,i](){
                emit deviceWasAttached(newDeviceInfoList.at(i));
            });
        }
    }

    //Check for detached devices
    for (int i = 0; i < m_lastScanResults.count(); i++) {
        if (!newDeviceInfoList.contains(m_lastScanResults.at(i))) {
            emit deviceWasDetached(m_lastScanResults.at(i));
        }
    }

    //Save last scan results for further refresh() calls
    m_lastScanResults = newDeviceInfoList;
}

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")#if defined(Q_OS_LINUX)
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
