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

#ifndef COMMANDLISTMANAGER_H
#define COMMANDLISTMANAGER_H

#include <QDateTime>
#include <QFileDevice>
#include <QFileInfo>
#include <QFileSystemWatcher>

class CommandList;

/*!
 *  @class CommandsListManager core/CommandsListManager.h
 *  @brief This class is responsible for creating, holding and deleting CommandList objects.
 */

class CommandsListManager : public QObject
{
    Q_OBJECT
public:
    explicit CommandsListManager(QObject* parent = nullptr);
    ~CommandsListManager();

    /*! @brief Creates new CommandList, delete old one (if any is existing) and reseting currentFileName to QString() */
    void      newCommandList();

    /*! @brief Open file fileName, load CommandList from it. If everything ok - replace old CommandList by new one.
     *         If not - does nothing */
    void      openCommandListFile(const QString& fileName);

    /*! @brief Reloads CommandList from currently opened file */
    void      reloadCurrentCommandFile();

    /*! @brief Saves current CommandList as fileName */
    void      saveCurrentCommandsListAs(const QString& fileName);

    /*! @brief Delets current CommandList and closes current file */
    void      closeCurrentFile();

    /*! @brief Returns true if any file is opened and current CommandList was loaded from it. */
    bool      fileOpened() const { return ( !m_currentFileInfo.fileName().isEmpty() ) && ( m_currentFileInfo.fileName() != defaultNewFileName() ); }

    /*! @brief Returns information about currently opened file */
    QFileInfo currentFileInfo() const { return m_currentFileInfo; }

    /*! @brief Returns translated default file name for creating new files */
    QString         defaultNewFileName() const                       { return tr("Untitled.cmds"); }

    /*! @brief Returns pointer to currently loaded CommandList. If none - nullptr */
    CommandList*    currentCommandsList()                            { return p_currentCommandList; }

signals:
    /*! @brief This signal is emitted in case of any error. Argument contains error description in human-readable form */
    void errorMessage(const QString& messageText);

    /*! @brief This signal is emitted when current CommandList is changed. */
    void commandListChanged(CommandList* newList);

    /*! @brief This signal is emitted when currentFile is changed. This can happen when new file was opened, or current data was saved as new file. */
    void currentFileInfoChanged(const QFileInfo& fileInfo);

    /*! @brief This signal is emitted when currently opened file was removed from disk by external program. */
    void commandsFileRemoved();

    /*! @brief This signal is emitted when currently opened file was modified on disk by any external program. */
    void commandsFileModified();

private slots:
    void _fileChangedWatcherSignal(const QString& filePath);

private:
    CommandList*   _createCommandListFromFile(const QString& fileName);

    void           _setCurrentCommandList(CommandList* newList);
    CommandList*   p_currentCommandList;

    void                _setCurrentFilePath(const QString& fileName);
    QFileSystemWatcher  m_fileWatcher;
    QFileInfo           m_currentFileInfo;
    QDateTime           m_lastFileModification;
};

#endif // COMMANDLISTMANAGER_H
