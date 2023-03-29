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

#include "CommandListManager.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "core/commands/CommandList.h"

CommandsListManager::CommandsListManager(QObject* parent)
       : QObject(parent),p_currentCommandList(nullptr)
{
    connect(&m_fileWatcher,&QFileSystemWatcher::fileChanged,this,&CommandsListManager::_fileChangedWatcherSignal);
}

CommandsListManager::~CommandsListManager()
{}

void CommandsListManager::newCommandList()
{
    _setCurrentCommandList(new CommandList);
    _setCurrentFilePath(defaultNewFileName());
}

void CommandsListManager::openCommandListFile(const QString& filePath)
{
    //Try opening file at filePath
    CommandList* newCmdList = _createCommandListFromFile(filePath);
    if (newCmdList == nullptr) {
        //If sth is wrong, and no file is opened - create a new file
        if ( ! fileOpened() ) {
            newCommandList();
        }
        return;
    }

    _setCurrentCommandList(newCmdList);
    _setCurrentFilePath(filePath);
}

void CommandsListManager::reloadCurrentCommandFile()
{
    if (m_currentFileInfo.fileName().isEmpty()) {
        return;
    }

    if (!m_currentFileInfo.exists()) {
        return;
    }

    CommandList* newCommands = _createCommandListFromFile(m_currentFileInfo.filePath());
    if (newCommands == nullptr) {
        //Some error happened, while parsing file, assume that sth was changed in command list
        p_currentCommandList->commandListChanged();
        return;
    }

    _setCurrentCommandList(newCommands);
}

void CommandsListManager::saveCurrentCommandsListAs(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorMessage(tr("Error opening file %1.\r\n%2").arg(fileName)
                          .arg(file.errorString()));
        return;
    }

    QJsonDocument jsonDoc;
    QJsonArray commandArray;

    QJsonObject rootObject;
    rootObject["commands"] = p_currentCommandList->toJsonArray();

    jsonDoc.setObject(rootObject);

    disconnect(&m_fileWatcher,&QFileSystemWatcher::fileChanged,this,&CommandsListManager::_fileChangedWatcherSignal);
    file.write(jsonDoc.toJson());
    file.flush();
    file.close();
    connect(&m_fileWatcher,&QFileSystemWatcher::fileChanged,this,&CommandsListManager::_fileChangedWatcherSignal);

    _setCurrentFilePath(fileName);

    return;
}

void CommandsListManager::closeCurrentFile()
{
    _setCurrentCommandList(nullptr);
    _setCurrentFilePath(QString());
}

void CommandsListManager::_fileChangedWatcherSignal(const QString& filePath)
{
    if (!QFile::exists(filePath)) {
        emit commandsFileRemoved();
        return;
    }

    m_currentFileInfo.refresh();

    if (m_currentFileInfo.lastModified() > m_lastFileModification) {
        m_lastFileModification = m_currentFileInfo.lastModified();
        emit commandsFileModified();
    }
}

CommandList* CommandsListManager::_createCommandListFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorMessage(tr("Error opening file %1.\r\n%2").arg(filePath)
                          .arg(file.errorString()));
        return nullptr;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll(),&jsonError);
    file.flush();
    file.close();

    if (jsonDocument.isNull()) {
        emit errorMessage(tr("Error while parsing file %1. \r\nJSON error: %2:%3").arg(filePath)
                          .arg(jsonError.errorString()).arg(jsonError.offset));
        return nullptr;
    }

    QJsonObject fileObject = jsonDocument.object();
    if (!fileObject.contains("commands") && !fileObject["commands"].isArray()) {
        emit errorMessage(tr("File %1 is valid JSON file, howewer - no \"commands\" JSON Array found.")
                          .arg(filePath));
        return nullptr;
    }

    QJsonArray commandsArray = fileObject["commands"].toArray();

    return CommandList::fromJsonArray(commandsArray);
}

void CommandsListManager::_setCurrentCommandList(CommandList* newList)
{
    if (p_currentCommandList != nullptr)
        p_currentCommandList->deleteLater();

    p_currentCommandList = newList;
    emit commandListChanged(p_currentCommandList);
}

void CommandsListManager::_setCurrentFilePath(const QString& filePath)
{
    //If file opened - we do not need to monotor it anymore
    if ( fileOpened() ) m_fileWatcher.removePath(m_currentFileInfo.fileName());

    m_currentFileInfo        = QFileInfo(filePath);
    m_lastFileModification   = m_currentFileInfo.lastModified();

    //If new file was opened - we need to start monitoring for changes in it
    if ( fileOpened() ) m_fileWatcher.addPath(filePath);

    //Inform about changes
    emit currentFileInfoChanged(m_currentFileInfo);
}
