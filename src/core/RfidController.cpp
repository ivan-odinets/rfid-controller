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

#include "RfidController.h"

#include "appconfig/RfidControllerSettings.h"
#include "commands/Command.h"
#include "commands/CommandList.h"

RfidController::RfidController(QObject *parent)
    : QObject(parent)
{
    connect(&m_commandListManager,&CommandsListManager::errorMessage,this,&RfidController::errorMessage);

    connect(&m_commandListManager,&CommandsListManager::commandListChanged,this,&RfidController::commandListChanged);
    connect(&m_commandListManager,&CommandsListManager::currentFileInfoChanged,this,&RfidController::commandFileNameChanged);
    connect(&m_commandListManager,&CommandsListManager::commandsFileRemoved,this,&RfidController::commandsFileRemoved);

    connect(&m_commandListManager,&CommandsListManager::commandsFileModified,this,&RfidController::commandsFileModified);

#ifdef HID
    connect(&m_inputDeviceManager,&InputDeviceManager::keyFound,this,&RfidController::_keyDiscovered);
    connect(&m_inputDeviceManager,&InputDeviceManager::errorMessage,this,&RfidController::errorMessage);

#ifdef LOG
    connect(&m_inputDeviceManager,&InputDeviceManager::inputDeviceWasAttached,&m_logger,&Logger::logAttachedInputDevice);
    connect(&m_inputDeviceManager,&InputDeviceManager::inputDeviceWasDetached,&m_logger,&Logger::logDetachedInputDevice);

    connect(&m_inputDeviceManager,&InputDeviceManager::inputDeviceWasOpened,&m_logger,&Logger::logOpenedInputDevice);
    connect(&m_inputDeviceManager,&InputDeviceManager::inputDeviceWasClosed,&m_logger,&Logger::logClosedInputDevice);

    connect(&m_inputDeviceManager,&InputDeviceManager::errorMessage,&m_logger,&Logger::logErrorMessage);
#endif //LOG

#endif //HID

#ifdef SERIAL
    connect(&m_serialDeviceManager,&SerialDeviceManager::keyFound,this,&RfidController::_keyDiscovered);
    connect(&m_serialDeviceManager,&SerialDeviceManager::errorMessage,this,&RfidController::errorMessage);

#ifdef LOG
    m_logger.currentLogFileChanged(RfidControllerSettings::get()->logFile());

    connect(&m_serialDeviceManager,&SerialDeviceManager::serialDeviceWasAttached,&m_logger,&Logger::logAttachedSerialDevice);
    connect(&m_serialDeviceManager,&SerialDeviceManager::serialDeviceWasDetached,&m_logger,&Logger::logDetachedSerialDevice);

    connect(&m_serialDeviceManager,&SerialDeviceManager::serialDeviceWasOpened,&m_logger,&Logger::logOpenedSerialDevice);
    connect(&m_serialDeviceManager,&SerialDeviceManager::serialDeviceWasClosed,&m_logger,&Logger::logClosedSerialDevice);

    connect(&m_serialDeviceManager,&SerialDeviceManager::errorMessage,&m_logger,&Logger::logErrorMessage);
#endif //LOG

#endif //SERIAL

#ifdef NFC
    connect(&m_nfcManager,&NfcManager::keyFound,this,&RfidController::_keyDiscovered);
    connect(&m_nfcManager,&NfcManager::errorMessage,this,&RfidController::errorMessage);
#endif
}

RfidController::~RfidController()
{
    if (m_commandListManager.fileOpened())
        RfidControllerSettings::get()->setOpenedCommandsFileName(m_commandListManager.currentFileInfo().absoluteFilePath());
    else
        RfidControllerSettings::get()->setOpenedCommandsFileName(QString());
}

void RfidController::start()
{
#ifdef LOG
    m_logger.start();
#endif //LOG

#ifdef HID
    m_inputDeviceManager.start();
#endif //HID

#ifdef SERIAL
    m_serialDeviceManager.start();
#endif //SERIAL

#ifdef NFC
    m_nfcManager.start();

    if (!m_nfcManager.isNfcSupported()) {
        qWarning() << "App was build with NFC support, hovever current device has no NFC!";
    }
#endif //NFC
}

void RfidController::_keyDiscovered(const QString& key)
{
    CommandList* cmdList = m_commandListManager.currentCommandsList();
    if (cmdList == nullptr) {
        qDebug() << "m_commandListManager.currentCommandsList() == nullptr";
        return;
    }

    for (int i = 0; i < cmdList->count(); i++) {
        Command* cmd = cmdList->at(i);
        if (cmd->key() == key) {
            cmd->run();
        }
    }

    emit keyFound(key);

#ifdef LOG
    m_logger.logKey(key);
#endif //LOG
}
