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

#ifndef NOTIFICATIONTYPE_H
#define NOTIFICATIONTYPE_H

#include <QtGlobal>
#include <QString>

/*!
 *  @class Notification core/NotificationType.h
 *  @brief This class stores information about type of notifications which app can show to user.
 */

class Notification
{
public:
    /*! @brief enum holding type of notification. */
    enum Type {
        None,                /*!< @brief No notifications. */
        MessageBox,          /*!< @brief Show notification as QMessageBox. */
#ifndef QT_NO_SYSTEMTRAYICON
        Systemtray,          /*!< @brief Show notification as QSystTray message. NOTE. This enum value is present only if building without
                              8          QT_NO_SYSTEMTRAYICON macro */
#endif //QT_NO_SYSTEMTRAYICON
        UnknownType          /*!< @brief Some unknown notification type. */
    };

    /*! @brief Helper static method which converts QString to Notification::Type value. If conversion is not possible -
     *         Type::UnknownType is returned. */
    static Type typeFromString(const QString& string);

    /*! @brief Helper static method which converts Notification::Type to QString. */
    static QString typeToString(Type type);
};

#endif // NOTIFICATIONTYPE_H
