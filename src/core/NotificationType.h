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

#ifndef NOTIFICATIONTYPE_H
#define NOTIFICATIONTYPE_H

#include <QtGlobal>
#include <QString>

class Notification
{
public:
    enum Type {
        None,
        MessageBox,
        Systemtray,
        UnknownType
    };

    static Type typeFromString(const QString& string);
    static QString typeToString(Type type);
};

#endif // NOTIFICATIONTYPE_H
