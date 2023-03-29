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

#include "NotificationType.h"

#define NO_NOTIFY_LITERAL    QStringLiteral("none")
#define MSG_NOTIFY_LITERAL   QStringLiteral("msg")

#ifndef QT_NO_SYSTEMTRAYICON
    #define TRAY_NOTIFY_LITERAL  QStringLiteral("tray")
#endif //QT_NO_SYSTEMTRAYICON

Notification::Type Notification::typeFromString(const QString& string) {
    if (string == NO_NOTIFY_LITERAL)
        return None;

    if (string == MSG_NOTIFY_LITERAL)
        return MessageBox;

#ifndef QT_NO_SYSTEMTRAYICON
    if (string == TRAY_NOTIFY_LITERAL)
        return Systemtray;
#endif //QT_NO_SYSTEMTRAYICON

    return UnknownType;
}

QString Notification::typeToString(Notification::Type type)
{
    switch (type) {
    case None:
        return NO_NOTIFY_LITERAL;
    case MessageBox:
        return MSG_NOTIFY_LITERAL;
#ifndef QT_NO_SYSTEMTRAYICON
    case Systemtray:
        return TRAY_NOTIFY_LITERAL;
#endif //QT_NO_SYSTEMTRAYICON
    case UnknownType:
        return QStringLiteral("unknown");
    }
    Q_ASSERT(false);
    return QString();
}
