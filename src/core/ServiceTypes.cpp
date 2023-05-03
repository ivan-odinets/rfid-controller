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

#include "ServiceTypes.h"

WordSet::WordSet(const QString& sourceString,const QChar& separator)
{
    QStringList parts = sourceString.split(separator);

    foreach (QString str, parts) {
        bool ok = false;
        quint16 value = str.toUInt(&ok,16);
        if (ok) {
            this->insert(value);
        }
    }
}

QString WordSet::toString(const QChar& separator) const
{
    if (this->isEmpty())
        return QString();

    QString result = QString();
    auto i = this->constBegin();
    result += QString::number(*i,16);
    i++;

    while (i != this->constEnd()) {
        result += separator;
        result += QString::number(*i,16);
        i++;
    }

    return result;
}

StringSet::StringSet(const QString& sourceString,const QChar& separator)
{
    if (sourceString.isEmpty())
        return;

    QStringList parts = sourceString.split(separator);
    foreach(QString str, parts) {
        this->insert(str);
    }

}

QString StringSet::toString(const QChar& separator) const
{
    return this->values().join(separator);
}
