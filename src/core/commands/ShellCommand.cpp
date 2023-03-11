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

#include "ShellCommand.h"

#include <QJsonArray>
#include <QProcess>

ShellCommand::ShellCommand(QObject* parent) :
    Command(parent)
{}

ShellCommand::ShellCommand(const QJsonObject& jsonObject, QObject* parent) :
    Command(jsonObject,parent),
    m_program(jsonObject.value("program").toString())
{
    for (QJsonValue jsonValue : jsonObject.value("arguments").toArray())
        m_arguments.append(jsonValue.toString());
}

void ShellCommand::setProgram(const QString& program)
{
    m_program = program;
    emit commandChanged();
}

void ShellCommand::setArguments(const QStringList& argumentsList)
{
    m_arguments = argumentsList;
    emit commandChanged();
}

void ShellCommand::setArgumentsString(const QString& argumentsString)
{
    m_arguments = argumentsString.split(' ');
}

QJsonObject ShellCommand::toJson() const
{
    return QJsonObject({
        { "type",        "shell" },
        { "enabled",     isEnabled() },
        { "key",         key() },
        { "program",     program() },
        { "arguments",   QJsonArray::fromStringList(arguments())}
    });
}

void ShellCommand::execute()
{
    qDebug() << "Executing command for key "<<this->key();
    QProcess::startDetached(m_program,m_arguments);
}
