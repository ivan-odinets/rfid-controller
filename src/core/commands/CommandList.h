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

#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <QObject>

#include <QJsonArray>

class Command;

/*!
 *  @class CommandList core/commands/CommandList.h
 *  @brief This class holds information about currently loaded Command objects, provides access to them and
 *         serialization to JSON.
 */

class CommandList : public QObject
{
    Q_OBJECT
public:
    explicit CommandList(QObject* parent = nullptr);
    ~CommandList();

    void      append(Command* cmd);
    int       count() const                                { return m_commandsList.count(); }

    void      clear();
    Command*  at(int index) const                          { return m_commandsList.at(index); }

    QJsonArray               toJsonArray() const;
    static CommandList*      fromJsonArray(const QJsonArray& array);

signals:
    void commandListChanged();
    void commandRemoved(Command* cmd);
    void commandAdded(Command* cmd);

public slots:
    void removeCommand(Command* cmd);

private:
    QList<Command*>  m_commandsList;
};

#endif // currentCommandsModel_H
