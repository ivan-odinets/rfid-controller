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

#ifndef INPUTDEVICEMANAGER_H
#define INPUTDEVICEMANAGER_H

#include <QObject>

#include "./appconfig/InputDeviceManagerSettings.h"

#include "./InputDeviceFilter.h"
#include "./InputDeviceWatcher.h"
#include "./core/devices/InputDevice.h"

/*!
 *  @class InputDeviceManager core/input/InputDeviceManager.h
 *  @brief This class is responsible for handling HID devices (manual opening, closure, autoconnection, etc.
 */

class InputDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit InputDeviceManager(QObject *parent = nullptr);
    ~InputDeviceManager() {}

public:
    void                start()                                 { m_inputDeviceWatcher.start(); }
    void                stop()                                  { m_inputDeviceWatcher.stop(); }
    void                updateInputDeviceList()                { m_inputDeviceWatcher.updateDeviceList(); }

    InputDeviceInfoList openedInputDevices() const;
    InputDeviceInfoList availableInputDevices() const           { return m_inputDeviceWatcher.getAvailableDevices(); }

    bool                inputDeviceAutoconnection() const       { return p_settings->inputDeviceAutoconnection(); }
    void                setInputDeviceAutoconnection(bool state);

    InputDeviceFilter   inputDeviceFilter() const               { return p_settings->inputDeviceFilter(); }
    void                setInputDeviceFilter(const InputDeviceFilter& newFilter);
    void                appendInputDeviceFilter(const InputDeviceFilter& sourceFilter);

signals:
    /*! @brief This signal is emitted when any of the connected devices has read any key/tag/etc. */
    void keyFound(const QString& key);

    /*! @brief This signal is emitted when any error has happeded. Contains description of the error in human-readable
     *         form. */
    void errorMessage(const QString& errorMessage);

    void inputDeviceWasOpened(const InputDeviceInfo& deviceDetails);
    void inputDeviceWasClosed(const InputDeviceInfo& deviceDetails);

    void inputDeviceWasAttached(const InputDeviceInfo& deviceDetails);
    void inputDeviceWasDetached(const InputDeviceInfo& deviceDetails);

public slots:
    /*! @brief This method is invoked when user has asked to open HID device */
    void inputDeviceOpeningRequested(const InputDeviceInfo& deviceDetails);

    /*! @brief This method is invoked when user has asked to close HID device */
    void inputDeviceClosureRequested(const InputDeviceInfo& deviceDetails);

private slots:
    void _handleAttachedInputDevice(const InputDeviceInfo& deviceDetails);
    void _handleDetachedInputDevice(const InputDeviceInfo& deviceDetails);
    void _handleClosedInputDevice();
    void _inputDeviceErrorOccured(InputDevice::InputDeviceError error);

private:
    InputDeviceManagerSettings*   p_settings;
    InputDeviceWatcher            m_inputDeviceWatcher;
    QList<InputDevice*>           m_openedInputDevices;
    bool _tryOpeningInputDevice(const InputDeviceInfo& portInfo);

    static QString _getInputErrorMessage(InputDevice::InputDeviceError error,const InputDeviceInfo& deviceDetails);
};

#endif // INPUTDEVICEMANAGER_H
