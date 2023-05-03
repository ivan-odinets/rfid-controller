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

#include "Logger.h"

#include <QCoreApplication>
#include <QFile>
#include <QDateTime>

#include "appconfig/LoggerSettings.h"

Logger::Logger(QObject *parent)
    : QObject{parent},p_logFile(nullptr)
{}

Logger::~Logger()
{
    if (p_logFile != nullptr) {
        p_logFile->close();
        p_logFile->deleteLater();
        p_logFile = nullptr;
    }
}

void Logger::start()
{
    if (!loggerSettings->logFile().isEmpty()) {
        createLogFile(loggerSettings->logFile());
    }

    _logString(QString("%1 started").arg(qApp->applicationName()));
}

void Logger::createLogFile(const QString& filePath)
{
    //We do not need to open empty file path
    if (filePath.isEmpty()) {
        qDebug() << "filePath.isEmpty() == true";
        return;
    }

    //Create new QFile object and try to open it
    QFile* newLogFile = new QFile(filePath);
    if (!newLogFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "p_logFile->open() failed...";
        _logString(tr("Error opening log file %1 for writing. Error string: %2")
                   .arg(filePath).arg(newLogFile->errorString()));
        newLogFile->deleteLater();
        return;
    }

    //File was opened, change settings parameter
    loggerSettings->setLogFile(filePath);
    m_logFileInfo = QFileInfo(filePath);

    //Handle old log file if needed
    if (p_logFile != nullptr) {
        p_logFile->close();
        p_logFile->deleteLater();
    }

    //Update pointer
    p_logFile = newLogFile;

    //Inform everybody about this
    emit currentLogFileChanged(m_logFileInfo);
    _logString(tr("Creating new log: %1").arg(filePath));
}

void Logger::openLogFile(const QString& filePath)
{
    //We do not need to open empty file path
    if (filePath.isEmpty()) {
        qDebug() << "filePath.isEmpty() == true";
        return;
    }

    //Create new QFile object and try to open it
    QFile* newLogFile = new QFile(filePath);
    if (!newLogFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "p_logFile->open() failed...";
        _logString(tr("Error opening log file %1 for writing. Error string: %2")
                   .arg(filePath).arg(newLogFile->errorString()));
        newLogFile->deleteLater();
        return;
    }

    //File was opened, change settings parameter
    loggerSettings->setLogFile(filePath);
    m_logFileInfo = QFileInfo(filePath);

    //Handle old log file if needed
    if (p_logFile != nullptr) {
        p_logFile->close();
        p_logFile->deleteLater();
    }

    //Update pointer
    p_logFile = newLogFile;

    emit currentLogFileChanged(m_logFileInfo);
    _logString(tr("Appending to log: %1").arg(filePath));
}

void Logger::closeLogFile()
{
    _logString(tr("Closing log file %1").arg(m_logFileInfo.filePath()));
    m_logFileInfo = QFileInfo();
    loggerSettings->setLogFile(QString());
    emit currentLogFileChanged(m_logFileInfo);

    if (p_logFile != nullptr) {
        p_logFile->close();
        p_logFile->deleteLater();
        p_logFile = nullptr;
    }
}

/*
 **********************************************************************************************************************
 * Configuring logger
 */

void Logger::setDiscoveredKeysLogging(bool state)
{
    loggerSettings->setKeysLogging(state);
}

bool Logger::logDiscoveredKeys() const
{
    return loggerSettings->logKeys();
}

void Logger::setMatchedKeysLogging(bool state)
{
    loggerSettings->setMatchedKeysLogging(state);
}

bool Logger::logMatchedKeys() const
{
    return loggerSettings->logMatchedKeys();
}

void Logger::setLoggingErrors(bool state)
{
    loggerSettings->setLoggingErrors(state);
}

bool Logger::logErrors() const
{
    return loggerSettings->logErrors();
}

bool Logger::logAttachedDevices() const
{
    return loggerSettings->logAttachedDevices();
}

void Logger::setAttachedDeviceLogging(bool state)
{
    loggerSettings->setAttachedDeviceLogging(state);
}

bool Logger::logDetachedDevices() const
{
    return loggerSettings->logDetachedDevices();
}

void Logger::setDetachedDeviceLogging(bool state)
{
    loggerSettings->setDetachedDeviceLogging(state);
}

bool Logger::logOpenedDevices() const
{
    return loggerSettings->logOpenedDevices();
}

void Logger::setOpenedDeviceLogging(bool state)
{
    loggerSettings->setOpenedDevicesLogging(state);
}

bool Logger::logClosedDevices() const
{
    return loggerSettings->logClosedDevices();
}

void Logger::setClosedDeviceLogging(bool state)
{
    loggerSettings->setClosedDeviceLogging(state);
}

/*
 **********************************************************************************************************************
 * Logging keys
 */

void Logger::logKey(const QString& key)
{
    if ( !(logDiscoveredKeys()) )
        return;

    QString keyString(tr("Key: %1").arg(key));

    _logString(keyString);
}

void Logger::logMatchedKey(const QString& key)
{
    if ( !(logMatchedKeys()) )
        return;

    QString keyString(tr("Key: %1").arg(key));

    _logString(keyString);
}

void Logger::logErrorMessage(const QString& errorMessage)
{
    if ( !(logErrors()) )
        return;

    _logString(errorMessage);
}

void Logger::_logString(const QString& logText)
{
    QString displayString("[");
    displayString.append(QDateTime::currentDateTime().toString("dd.MM.yyyy - hh:mm:ss"));
    displayString.append("] ");
    displayString.append(logText);

    //Emit signal that we are doing sth
    emit loggerEvent(displayString);

    if (p_logFile == nullptr) {
        return;
    }

    if (!p_logFile->isOpen()) {
        return;
    }

    p_logFile->write(displayString.toLocal8Bit());
    p_logFile->write("\n");
    p_logFile->flush();
}

QString Logger::_defaultLogPath()
{
    return QString(qApp->applicationDirPath()+QDir::separator()+"events.log");
}

/*
 **********************************************************************************************************************
 * This part is needed only if we have HID support enabled
 */

#ifdef HID

void Logger::logAttachedInputDevice(const InputDeviceInfo& deviceInfo)
{
    if ( !(logAttachedDevices()) )
        return;

    _logString(tr("HID device attached (vendorId=%1; productId=%2; systemPath=%3")
               .arg(deviceInfo.vendorId())
               .arg(deviceInfo.productId())
               .arg(deviceInfo.deviceFilePath()));
}

void Logger::logDetachedInputDevice(const InputDeviceInfo& deviceInfo)
{
    if ( !(logDetachedDevices()) )
        return;

    _logString(tr("HID device detached (vendorId=%1; productId=%2; systemPath=%3")
               .arg(deviceInfo.vendorId())
               .arg(deviceInfo.productId())
               .arg(deviceInfo.deviceFilePath()));
}

void Logger::logOpenedInputDevice(const InputDeviceInfo& deviceInfo)
{
    if ( !(logOpenedDevices()) )
        return;

    _logString(tr("HID device opened (vendorId=%1; productId=%2; systemPath=%3")
               .arg(deviceInfo.vendorId())
               .arg(deviceInfo.productId())
               .arg(deviceInfo.deviceFilePath()));
}

void Logger::logClosedInputDevice(const InputDeviceInfo& deviceInfo)
{
    if ( !(logClosedDevices()) )
        return;

    _logString(tr("HID device closed (vendorId=%1; productId=%2; systemPath=%3")
               .arg(deviceInfo.vendorId())
               .arg(deviceInfo.productId())
               .arg(deviceInfo.deviceFilePath()));
}

#endif //HID

/*
 **********************************************************************************************************************
 * This part is needed only if we have Serial support enabled
 */

#ifdef SERIAL

void Logger::logAttachedSerialDevice(const QSerialPortInfo& portInfo)
{
    if ( !(logAttachedDevices()) )
        return;

    _logString(tr("Serial device attached (vendorId=%1; productId=%2; portName=%3")
               .arg(portInfo.vendorIdentifier())
               .arg(portInfo.productIdentifier())
               .arg(portInfo.portName()));
}

void Logger::logDetachedSerialDevice(const QSerialPortInfo& portInfo)
{
    if ( !(logDetachedDevices()) )
        return;

    _logString(tr("Serial device detached (vendorId=%1; productId=%2; portName=%3")
               .arg(portInfo.vendorIdentifier())
               .arg(portInfo.productIdentifier())
               .arg(portInfo.portName()));
}

void Logger::logOpenedSerialDevice(const QSerialPortInfo& portInfo)
{
    if ( !(logOpenedDevices()) )
        return;

    _logString(tr("Serial device opened (vendorId=%1; productId=%2; portName=%3")
               .arg(portInfo.vendorIdentifier())
               .arg(portInfo.productIdentifier())
               .arg(portInfo.portName()));
}

void Logger::logClosedSerialDevice(const QSerialPortInfo& portInfo)
{
    if ( !(logClosedDevices()) )
        return;

    _logString(tr("Serial device closed (vendorId=%1; productId=%2; portName=%3")
               .arg(portInfo.vendorIdentifier())
               .arg(portInfo.productIdentifier())
               .arg(portInfo.portName()));
}

#endif //SERIAL
