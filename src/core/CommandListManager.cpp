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
    _setCurrentFileName(defaultNewFileName());
}

void CommandsListManager::openCommandListFile(const QString& fileName)
{
    CommandList* newCmdList = _createCommandListFromFile(fileName);
    if (newCmdList == nullptr)
        return;

    _setCurrentCommandList(newCmdList);
    _setCurrentFileName(fileName);
}

void CommandsListManager::reloadCurrentCommandFile()
{
    if (m_currentFileName.isEmpty()) {
        return;
    }

    if (!QFile::exists(m_currentFileName)) {
        return;
    }

    CommandList* newCommands = _createCommandListFromFile(m_currentFileName);
    if (newCommands == nullptr) {
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

    file.write(jsonDoc.toJson());
    file.flush();
    file.close();

    _setCurrentFileName(fileName);

    return;
}

void CommandsListManager::closeCurrentFile()
{
    _setCurrentCommandList(nullptr);
    _setCurrentFileName(QString());
}

void CommandsListManager::_fileChangedWatcherSignal(const QString& filePath)
{
    if (!QFile::exists(filePath)) {
        emit commandsFileRemoved();
        return;
    }
}

CommandList* CommandsListManager::_createCommandListFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorMessage(tr("Error opening file %1.\r\n%2").arg(fileName)
                          .arg(file.errorString()));
        return nullptr;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll(),&jsonError);

    if (jsonDocument.isNull()) {
        emit errorMessage(tr("Error while parsing file %1. \r\nJSON error: %2:%3").arg(fileName)
                          .arg(jsonError.errorString()).arg(jsonError.offset));
        return nullptr;
    }

    QJsonObject fileObject = jsonDocument.object();
    if (!fileObject.contains("commands") && !fileObject["commands"].isArray()) {
        emit errorMessage(tr("File %1 is valid JSON file, howewer - no \"commands\" JSON Array found.")
                          .arg(fileName));
        return nullptr;
    }

    QJsonArray commandsArray = fileObject["commands"].toArray();
    file.flush();
    file.close();

    return CommandList::fromJsonArray(commandsArray);
}

void CommandsListManager::_setCurrentCommandList(CommandList* newList)
{
    if (p_currentCommandList != nullptr)
        p_currentCommandList->deleteLater();

    p_currentCommandList = newList;
    emit commandListChanged(p_currentCommandList);
}

void CommandsListManager::_setCurrentFileName(const QString& fileName)
{
    if ( (!m_currentFileName.isEmpty()) && (m_currentFileName != defaultNewFileName()) )
        m_fileWatcher.removePath(m_currentFileName);

    if ( (!fileName.isEmpty()) && (fileName != defaultNewFileName()) )
        m_fileWatcher.addPath(fileName);

    m_currentFileName = fileName;
    emit currentFileNameChanged(m_currentFileName);
}
