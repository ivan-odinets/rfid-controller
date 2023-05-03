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

#ifndef NOTIFICATIONTYPEMENU_H
#define NOTIFICATIONTYPEMENU_H

#include <QMenu>

#include "core/NotificationType.h"

/*!
 *  @class NotificationMenu menus/NotificationTypeMenu.h
 *  @brief This class allows creation of menus, which allow user to select type of notifications represented by values
 *         from Notification::Type enum.
 *  @details When building without systray support (macro QT_NO_SYSTEMTRAYICON defined) menu entry responsible for
 *           Notification::Type::Systemtray will be absent.
 */

class NotificationMenu : public QMenu
{
    Q_OBJECT
public:
    explicit NotificationMenu(QWidget *parent = nullptr);
    explicit NotificationMenu(const QString& title, QWidget* parent = nullptr);

    /*! @brief Use this method to display Notification::Type value. No signals are emitted. */
    void setNotificationType(Notification::Type newStatus);

signals:
    /*! @brief This signal is emitted when user changed type of notification. */
    void notificationTypeChanged(Notification::Type notify);

private slots:
    void _notificationActionSelected(QAction* action);

private:
    inline void _setupUi();

    QAction* w_noNotificationAction;
    QAction* w_messageBoxNotificationAction;

#ifndef QT_NO_SYSTEMTRAYICON
    QAction* w_systrayNotificationAction;
#endif //QT_NO_SYSTEMTRAYICON
};

#endif // NOTIFICATIONTYPEMENU_H
