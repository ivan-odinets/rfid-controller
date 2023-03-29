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

#ifndef NOTIFICATIONTYPEMENU_H
#define NOTIFICATIONTYPEMENU_H

#include <QMenu>

#include "core/NotificationType.h"

class NotificationMenu : public QMenu
{
    Q_OBJECT
public:
    explicit NotificationMenu(QWidget *parent = nullptr);
    explicit NotificationMenu(const QString& title, QWidget* parent = nullptr);

    void setNotificationType(Notification::Type newStatus);

signals:
    void notificationTypeChanged(Notification::Type notify);

private slots:
    void _notificationActionSelected(QAction* action);

private:
    void _setupUi();

    QAction* w_noNotificationAction;
    QAction* w_messageBoxNotificationAction;

#ifndef QT_NO_SYSTEMTRAYICON
    QAction* w_systrayNotificationAction;
#endif //QT_NO_SYSTEMTRAYICON
};

#endif // NOTIFICATIONTYPEMENU_H
