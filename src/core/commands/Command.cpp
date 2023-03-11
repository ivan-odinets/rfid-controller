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

#include "Command.h"

#include "ShellCommand.h"

Command::Command(QObject* parent) :
    QObject(parent)
{}

Command::Command(const QJsonObject& jsonObject, QObject* parent) :
    QObject(parent),
    m_enabled(jsonObject.value("enabled").toBool()),
    m_key(jsonObject.value("key").toString())
{}

void Command::run()
{
    if (!m_enabled)
        return;

    this->execute();
}

void Command::setKey(const QString& key)
{
    m_key = key;
    emit commandChanged();
}

void Command::setEnabled(bool state)
{
    m_enabled = state;
    emit commandChanged();
}

Command* Command::create(Command::Type type)
{
    switch (type) {
    case Shell:
        return new ShellCommand;
    case Unknown:
        return nullptr;
    }
    Q_ASSERT(false);
    return nullptr;
}

Command* Command::fromJson(const QJsonObject& jsonObject)
{
    QString type = jsonObject.value("type").toString();

    if (type == QLatin1String("shell")) {
        return new ShellCommand(jsonObject);
    }

    return nullptr;
}
