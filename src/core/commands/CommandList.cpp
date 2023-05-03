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

#include "CommandList.h"

#include <QDebug>
#include <QJsonArray>

#include "Command.h"

CommandList::CommandList(QObject *parent) :
    QObject(parent)
{}

CommandList::~CommandList()
{
    clear();
}

void CommandList::append(Command* cmd)
{
    m_commandsList.append(cmd);
    emit commandListChanged();
    emit commandAdded(cmd);
    connect(cmd,&Command::commandChanged,this,&CommandList::commandListChanged);
}

void CommandList::clear()
{
    for (Command* cmd : m_commandsList) {
        emit commandRemoved(cmd);
        cmd->deleteLater();
    }

    m_commandsList.clear();
    emit commandListChanged();
}

QJsonArray CommandList::toJsonArray() const
{
    QJsonArray result;

    for (int i = 0; i < count(); i++) {
        if (!at(i)->key().isEmpty())
            result.append(at(i)->toJson());
    }

    return result;
}

CommandList* CommandList::fromJsonArray(const QJsonArray& array)
{
    CommandList* result = new CommandList;

    for (int i = 0; i < array.count(); i++) {
        Command* cmd = Command::fromJson(array.at(i).toObject());
        if (cmd == nullptr)
            continue;

        if (cmd->key().isEmpty())
            continue;

        result->append(cmd);
    }

    return result;
}

void CommandList::removeCommand(Command* cmd)
{
    m_commandsList.removeAll(cmd);
    emit commandRemoved(cmd);
    emit commandListChanged();

    cmd->deleteLater();
}
