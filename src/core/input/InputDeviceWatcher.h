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

#ifndef INPUTDEVICEWATCHER_H
#define INPUTDEVICEWATCHER_H

#include <QObject>
#include <QStringList>

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    #include <QDir>
    #include <QFileSystemWatcher>
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif

#include "InputDeviceInfo.h"

/*!
 * @class InputDeviceWatcher core/input/InputDeviceWatcher.h
 * @brief This class is responsible for monitoring system for newly attached/or detached HID devices
 * @details Emitting signal InputDeviceWatcher::deviceWasAttached when some HID device is atatched.
 *          And signal InputDeviceWatcher::deviceWasDetached when HID device was detached.
 */

class InputDeviceWatcher : public QObject
{
    Q_OBJECT
public:
    explicit InputDeviceWatcher(QObject* parent = nullptr);
    ~InputDeviceWatcher() {}

    void start();
    void stop();
    void updateDeviceList();

    InputDeviceInfoList availableDevices() const { return m_lastScanResults; }

signals:
    void deviceWasAttached(const InputDeviceInfo& deviceDetails);
    void deviceWasDetached(const InputDeviceInfo& deviceDetails);

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of InputDeviceWatcher
//

private:
    QFileSystemWatcher      m_fileSystemWatcher;
    InputDeviceInfoList     m_lastScanResults;

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
};

#endif // INPUTDEVICEWATCHER_H
