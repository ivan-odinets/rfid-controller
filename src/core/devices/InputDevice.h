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

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <QObject>

#if defined(Q_OS_LINUX) && !defined (Q_OS_ANDROID)
    #include <QSocketNotifier>
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC

#include "./core/input/InputDeviceInfo.h"
#include "./core/input/InputEvent.h"

/*!
 * @class InputDevice devices/InputDevice.h
 * @brief Represents an input device.
 */

class InputDevice : public QObject
{
    Q_OBJECT
public:
    enum OpenMode {
        ReadOnly,
        WriteOnly,
        ReadWrite
    };

    /*! @brief This enum holds information about poissible errors, which can happen while working with HID devices */
    enum InputDeviceError {
        NoError            = 0x0000,
        PermissionError,
        DeviceIsBusy,
        NoSuchDeviceError,
        UnknownError,
    };

    explicit InputDevice(QObject *parent = nullptr);
    explicit InputDevice(const InputDeviceInfo& deviceInfo, QObject* parent = nullptr);
    ~InputDevice();

    bool open(OpenMode mode);
    void close();
    bool isOpened() const;

    void              setDeviceInfo(const InputDeviceInfo& deviceDetails)      { m_deviceDetails = deviceDetails; }
    InputDeviceInfo   deviceInfo() const                                       { return m_deviceDetails; }

    InputDeviceError  error() const                                            { return m_errorCode; }

    /*! @brief This method returns error in human-readable form */
    QString           errorString() const                                      { return m_errorMessage; }

    void              setExclusiveAccess(bool status);
    bool              hasExclusiveAccess() const;

signals:
    void keyFound(const QString& keyString);

    void deviceOpened();

    /*! @brief This signal is emitted when device is closed (because of whatever reason) */
    void deviceClosed();

    /*! @brief This signal is emitted whatever device error has happened */
    void errorOccured(InputDeviceError error);

protected:
    void _setErrorState(const QString& msg,int errorCode);

    /*! @brief This method is called when some input event is happening */
    void _processInputEvent(const InputEvent& event);

private:
    InputDeviceInfo     m_deviceDetails;
    bool                m_exclusiveAccess;
    QString             m_errorMessage;
    InputDeviceError    m_errorCode;

    QByteArray          m_inputBuffer;

#if defined(Q_OS_LINUX) && !defined (Q_OS_ANDROID)
//
// Linux-specific part of InputDevice
//

private slots:
    void _dataReadyForReading(int socket);

private:
    int                 m_deviceHandler;
    struct libevdev*    p_libevdev;
    int                 m_libevdevId;
    QSocketNotifier*    p_socketNotifier;

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
};

#endif // INPUTDEVICE_H
