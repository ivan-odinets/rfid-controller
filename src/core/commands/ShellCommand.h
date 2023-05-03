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

#ifndef SHELLCOMMAND_H
#define SHELLCOMMAND_H

#include "Command.h"

/*!
 *  @class ShellCommand core/commands/ShellCommand.h
 *  @brief This class implements launching processes, scripts and other simple stuff on computer.
 */

class ShellCommand : public Command
{
    Q_OBJECT
public:
    explicit ShellCommand(QObject* parent = nullptr);
    explicit ShellCommand(const QJsonObject& jsonObject, QObject* parent = nullptr);
    ~ShellCommand() {}

    /*! @brief Type of this object. Represented by Shell value from Command::Type enum */
    Type type() const override { return Shell; };

    void           setProgram(const QString& program);
    QString        program() const                              { return m_program; }

    void           setArguments(const QStringList& arguments);
    QStringList    arguments() const                            { return m_arguments; }

    void           setArgumentsString(const QString& argumentsString);
    QString        argumentsString() const                      { return m_arguments.join(' '); }

    /*! @brief Serialize this ShellCommand object to JSON. */
    QJsonObject    toJson() const override;

protected:
    void           execute() override;

private:
    Q_DISABLE_COPY(ShellCommand);
    QString        m_program;
    QStringList    m_arguments;
};

#endif // SHELLCOMMAND_H
