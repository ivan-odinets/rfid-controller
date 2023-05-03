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

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

#include <QFile>
#include <QFileInfo>

#ifdef HID
    #include "core/input/InputDeviceInfo.h"
#endif //HID

#ifdef SERIAL
    #include <QSerialPortInfo>
#endif //SERIAL

/*!
 *  @class Logger core/Logger.h
 *  @brief This class is responsible for logging events inside the app.
 */

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    /*! @brief This method can be used to write to log file that application has started. */
    void start();

    /*! @brief This method creates new log file and redirects logging to it. Argument - path to log file. */
    void createLogFile(const QString& filePath);

    /*! @brief This method opens existing log file and appends new output to it. Argument - path to log file.  */
    void openLogFile(const QString& filePath);

    /*! @brief This method closes current log file. */
    void closeLogFile();

    /*! @brief This method returns information about currently opened log file. */
    QFileInfo logFile() const     { return m_logFileInfo; }

    /*! @brief Returns true if this Logger object will log discovered keys. */
    bool logDiscoveredKeys() const;

    /*! @brief Returns true if this Logger object will log matched keys. */
    bool logMatchedKeys() const;

    /*! @brief Returns true if this Logger object will log errors. */
    bool logErrors() const;

    /*! @brief Returns true if this Logger object will log attached devices. */
    bool logAttachedDevices() const;

    /*! @brief Returns true if this Logger object will log detached devices. */
    bool logDetachedDevices() const;

    /*! @brief Returns true if this Logger object will log opened devices. */
    bool logOpenedDevices() const;

    /*! @brief Returns true if this Logger object will log closed devices. */
    bool logClosedDevices() const;

signals:
    /*! @brief This signal is emitted when current log file changes. */
    void currentLogFileChanged(const QFileInfo& newFileInfo);

    /*! @brief This signal is emitted when Logger class has recieved information about some event, which needs to be
     *         placed to log. Or in case of some Logger internal events, which needs to be displayed to user. */
    void loggerEvent(const QString& line);

public slots:
    /*! @brief Pass true to enable logging of discovered keys. */
    void setDiscoveredKeysLogging(bool state);

    /*! @brief Pass true to enable logging of matched keys. */
    void setMatchedKeysLogging(bool state);

    /*! @brief Pass true to enable logging of errors. */
    void setLoggingErrors(bool state);

    /*! @brief Pass true to enable logging of attached devices. */
    void setAttachedDeviceLogging(bool state);

    /*! @brief Pass true to enable logging of detached devices. */
    void setDetachedDeviceLogging(bool state);

    /*! @brief Pass true to enable logging of opened devices. */
    void setOpenedDeviceLogging(bool state);

    /*! @brief Pass true to enable logging of closed devices. */
    void setClosedDeviceLogging(bool state);

    /*! @brief This slot should be invoked to log discovered key. */
    void logKey(const QString& key);

    /*! @brief This slot should be invoked to log matched key. */
    void logMatchedKey(const QString& key);

    /*! @brief This slot should be invoked to log error message. */
    void logErrorMessage(const QString& errorMessage);

private:
    static QString _defaultLogPath();
    void           _logString(const QString& logText);

    QFileInfo m_logFileInfo;
    QFile*    p_logFile;

#ifdef HID
//
// This part is needed only if we have HID support enabled
//

public slots:
    /*! @brief This slot should be invoked to log attached HID device. */
    void logAttachedInputDevice(const InputDeviceInfo& deviceInfo);

    /*! @brief This slot should be invoked to log detached HID device. */
    void logDetachedInputDevice(const InputDeviceInfo& deviceInfo);

    /*! @brief This slot should be invoked to log opened HID device. */
    void logOpenedInputDevice(const InputDeviceInfo& deviceInfo);

    /*! @brief This slot should be invoked to log closed HID device. */
    void logClosedInputDevice(const InputDeviceInfo& deviceInfo);

#endif //HID

#ifdef SERIAL
//
// This part is needed only if we have Serial support enabled
//

public slots:
    /*! @brief This slot should be invoked to log attached serial device. */
    void logAttachedSerialDevice(const QSerialPortInfo& portInfo);

    /*! @brief This slot should be invoked to log detached serial device. */
    void logDetachedSerialDevice(const QSerialPortInfo& portInfo);

    /*! @brief This slot should be invoked to log opened serial device. */
    void logOpenedSerialDevice(const QSerialPortInfo& portInfo);

    /*! @brief This slot should be invoked to log closed serial device. */
    void logClosedSerialDevice(const QSerialPortInfo& portInfo);

#endif //SERIAL
};

#endif // LOGGER_H
