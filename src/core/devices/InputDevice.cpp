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

#include "InputDevice.h"

#include <QDebug>

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    #include <QFile>
    #include <QSocketNotifier>

    #include <errno.h>
    #include <fcntl.h>
    #include <linux/input.h>
    #include <unistd.h>
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#elif
    #error("Builds for other platforms are not supported")
#endif

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific code
//

InputDevice::InputDevice(QObject *parent) : QObject(parent),
    m_exclusiveAccess(true),
    m_errorCode(NoError),
    m_deviceHandler(-1),
    p_libevdev(nullptr),
    p_socketNotifier(nullptr)
{}

InputDevice::InputDevice(const InputDeviceInfo& deviceInfo, QObject* parent) : QObject(parent),
    m_deviceDetails(deviceInfo),
    m_exclusiveAccess(true),
    m_errorCode(NoError),
    m_deviceHandler(-1),
    p_libevdev(nullptr),
    p_socketNotifier(nullptr)
{}

InputDevice::~InputDevice()
{
    if (m_deviceHandler > 0)
        ::close(m_deviceHandler);

    if (p_socketNotifier != nullptr)
        p_socketNotifier->deleteLater();
}

bool InputDevice::open(OpenMode mode)
{
    m_deviceHandler = ::open(m_deviceDetails.deviceFilePath().toStdString().c_str(), mode);
    if (m_deviceHandler < 0) {
        qDebug() << "open() call failed for device: "<<m_deviceDetails.deviceFileName()<<"; errno="<<errno<<"; strerror(errno)="<<strerror(errno);
        _setErrorState(QString(strerror(errno)),errno);
        return false;
    }

    setExclusiveAccess(m_exclusiveAccess);

    p_socketNotifier = new QSocketNotifier(m_deviceHandler,QSocketNotifier::Read);
    connect(p_socketNotifier,&QSocketNotifier::activated,this,&InputDevice::_dataReadyForReading);
    p_socketNotifier->setEnabled(true);

    emit deviceOpened();
    return true;
}

void InputDevice::close()
{
    if (p_socketNotifier != nullptr) {
        p_socketNotifier->setEnabled(false);
        p_socketNotifier->deleteLater();
        p_socketNotifier = nullptr;
    }

    ::close(m_deviceHandler);
    m_deviceHandler = -1;
    emit deviceClosed();
}

bool InputDevice::isOpened() const
{
    return (m_deviceHandler > 0);
}

void InputDevice::setExclusiveAccess(bool status)
{
    m_exclusiveAccess = status;

    if (this->isOpened()) {
        int result = ioctl(m_deviceHandler, EVIOCGRAB, (status) ? 1 : 0);
        qDebug() << m_deviceDetails.deviceFileName()<<", getting exclusive access: "<<((result == 0) ? "SUCCESS" : "FAILURE");
        if (result != 0) {
            qDebug() << "ioctl() call failed for device: "<<m_deviceDetails.deviceFileName()<<"; errno="<<errno<<"; strerror(errno)="<<strerror(errno);
            _setErrorState(strerror(errno),errno);
        }
    }
}

bool InputDevice::hasExclusiveAccess() const
{
    return m_exclusiveAccess;
}

void InputDevice::_setErrorState(const QString& msg,int errorCode)
{
    m_errorMessage = msg;
    switch (errorCode) {
    case EACCES:
        m_errorCode = PermissionError;
        break;
    case EBUSY:
        m_errorCode = DeviceIsBusy;
        break;
    case ENODEV:
        m_errorCode = NoSuchDeviceError;
        break;
    default:
        m_errorCode = UnknownError;
        break;
    }

    emit errorOccured(m_errorCode);

    close();
}

void InputDevice::_processInputEvent(const InputEvent& event)
{
    if (event.type() != InputEvent::Key) {
        return;
    }

    if (event.value() != 1) {
        return;
    }

    //----- KEY DOWN -----
    char key = event.charCode();
    m_inputBuffer.append(key);
    if (key == '\0') {
        emit keyFound(QString(m_inputBuffer));
        m_inputBuffer.clear();
        return;
    }
}

void InputDevice::_dataReadyForReading(int socket)
{
    Q_ASSERT(socket == m_deviceHandler);

    struct input_event event;

    int reading = read(m_deviceHandler, &event, sizeof(struct input_event));
    if (reading < (int) sizeof(struct input_event)) {
        qDebug() << "read() call failed for device: "<<m_deviceDetails.deviceFileName()<<"; errno="<<errno<<"; strerror(errno)="<<strerror(errno);
        //We need to disable p_socketNotifier, otherwise somehow QSocketNotifier::activated() signal is continiuosly emitted.
        p_socketNotifier->setEnabled(false);
        //Than - emit errorOccured() signal and set some reasonable errorMessage text
        _setErrorState(strerror(errno),errno);
        return;
    }

    _processInputEvent(InputEvent(event));
}

#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#elif
    #error("Builds for other platforms are not supported")
#endif
