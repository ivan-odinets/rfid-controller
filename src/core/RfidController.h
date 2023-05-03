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

#ifndef RFIDCONTROLLER_H
#define RFIDCONTROLLER_H

#include <QObject>

#include "core/CommandListManager.h"

#ifdef HID
    #include "core/input/InputDeviceManager.h"
#endif //HID

#ifdef SERIAL
    #include "core/serial/SerialDeviceManager.h"
#endif //SERIAL

#ifdef NFC
    #include "core/nfc/NfcManager.h"
#endif //NFC

#ifdef LOG
    #include "Logger.h"
#endif //LOG

/*!
 *  @class RfidController core/RfidController.h
 *  @brief Main class where almost everything is happening.
 */

class RfidController : public QObject
{
    Q_OBJECT
public:
    static RfidController* get() {
        static RfidController theOne;
        return &theOne;
    }
    ~RfidController();

    void start();

    /*! @brief This method creates new CommandList. See CommandListManager::newCommandList. */
    void           newCommandList()                                       { m_commandListManager.newCommandList(); }

    /*! @brief Open file fileName, load CommandList from it. See CommandListManager::openCommandListFile. */
    void           openCommandListFile(const QString& fileName)           { m_commandListManager.openCommandListFile(fileName); }

    /*! @brief Reloads CommandList from currently opened file See CommandListManager::reloadCurrentCommandFile. */
    void           reloadCurrentCommandFile()                             { m_commandListManager.reloadCurrentCommandFile(); }

    /*! @brief Saves current CommandList as fileName. See CommandListManager::saveCurrentCommandsListAs. */
    void           saveCurrentCommandsListAs(const QString& fileName)     { m_commandListManager.saveCurrentCommandsListAs(fileName); }

    /*! @brief Deletes current CommandList and closes current file. See CommandListManager::closeCurrentFile. */
    void           closeCurrentFile()                                     { m_commandListManager.closeCurrentFile(); };
    QFileInfo      currentFileInfo() const                                { return m_commandListManager.currentFileInfo(); }
    QString        defaultNewFileName() const                             { return tr("Untitled.cmds"); }
    CommandList*   currentCommandsList()                                  { return m_commandListManager.currentCommandsList(); }

signals:
    /*! @brief This signal is emitted when any key was read by any of the connected devices */
    void keyFound(const QString& key);

    /*! @brief This signal is emitted when any error happened. Argument - error description in human-readable form */
    void errorMessage(const QString& errorMessage);

    /*! @brief This signal is emitted when currently opened file was changed on disk by external program */
    void commandsFileModified();

    /*! @brief This signal is emitted when currently opened file was removed from disk by external program */
    void commandsFileRemoved();

//    /*! @brief This signal is emitted when name of currently opened file was changed. The content can be the same */
//    void commandsFileNameChanged(const QString& fileName);

    void commandFileNameChanged(const QFileInfo& fileInfo);

    /*! @brief This signal is emitted when current command list was totally changed */
    void commandListChanged(CommandList* model);

private slots:
    void _keyDiscovered(const QString& key);

private:
    explicit RfidController(QObject *parent = nullptr);
    Q_DISABLE_COPY(RfidController);

    CommandsListManager      m_commandListManager;

#ifdef HID
//
// This part is needed only if we have HID support enabled
//

public:
    InputDeviceManager* inputDeviceManager() { return &m_inputDeviceManager; }

private:
    InputDeviceManager  m_inputDeviceManager;
#endif //HID

#ifdef SERIAL
//
// This part is needed only if we have Serial support enabled
//

public:
    SerialDeviceManager* serialDeviceManager() { return &m_serialDeviceManager; }

private:
    SerialDeviceManager  m_serialDeviceManager;
#endif //SERIAL

#ifdef NFC
//
// This part is needed only if we have NFC support enabled
//

public:
    NfcManager*         nfcManager() { return &m_nfcManager; }

private:
    NfcManager          m_nfcManager;
#endif //NFC

#ifdef LOG
//
// This part is needed only if we have Logging support enabled
//

public:
    Logger*             logger() { return &m_logger; }

private:
    Logger              m_logger;
#endif //LOG
};

#endif // RFIDCONTROLLER_H
