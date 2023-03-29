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
#endif //HID

#ifdef SERIAL
    connect(&m_serialDeviceManager,&SerialDeviceManager::keyFound,this,&RfidController::_keyDiscovered);
    connect(&m_serialDeviceManager,&SerialDeviceManager::errorMessage,this,&RfidController::errorMessage);

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
}
