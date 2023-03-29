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

#include "NotificationTypeMenu.h"

#include <QAction>
#include <QActionGroup>

NotificationMenu::NotificationMenu(QWidget *parent)
    : QMenu{parent}
{
    _setupUi();
}

NotificationMenu::NotificationMenu(const QString& title,QWidget *parent)
    : QMenu{title,parent}
{
    _setupUi();
}

void NotificationMenu::setNotificationType(Notification::Type newStatus)
{
    switch (newStatus) {
    case Notification::UnknownType:
    case Notification::None:
        w_noNotificationAction->setChecked(true);
        return;
    case Notification::MessageBox:
        w_messageBoxNotificationAction->setChecked(true);
        return;
    case Notification::Systemtray:
        w_systrayNotificationAction->setChecked(true);
        return;
    }
    Q_ASSERT(false);
}

void NotificationMenu::_notificationActionSelected(QAction* action)
{
    switch (action->data().toInt()) {
    case Notification::None:
        emit notificationTypeChanged(Notification::None);
        return;
    case Notification::MessageBox:
        emit notificationTypeChanged(Notification::MessageBox);
        return;
#ifndef QT_NO_SYSTEMTRAYICON
    case Notification::Systemtray:
        emit notificationTypeChanged(Notification::Systemtray);
        return;
#endif //QT_NO_SYSTEMTRAYICON
    }
    Q_ASSERT(false);
}

void NotificationMenu::_setupUi()
{
    QActionGroup* w_notificationActionGroup = new QActionGroup(this);
    w_notificationActionGroup->setExclusive(true);

    w_noNotificationAction = addAction(tr("None"));
    w_noNotificationAction->setData(Notification::None);
    w_noNotificationAction->setCheckable(true);
    w_notificationActionGroup->addAction(w_noNotificationAction);

    w_messageBoxNotificationAction = this->addAction(tr("MessageBox"));
    w_messageBoxNotificationAction->setData(Notification::MessageBox);
    w_messageBoxNotificationAction->setCheckable(true);
    w_notificationActionGroup->addAction(w_messageBoxNotificationAction);

#ifndef QT_NO_SYSTEMTRAYICON
    w_systrayNotificationAction = addAction(tr("Tray"));
    w_systrayNotificationAction->setData(Notification::Systemtray);
    w_systrayNotificationAction->setCheckable(true);
    w_notificationActionGroup->addAction(w_systrayNotificationAction);
#endif //QT_NO_SYSTEMTRAYICON

    connect(w_notificationActionGroup,&QActionGroup::triggered,this,&NotificationMenu::_notificationActionSelected);
}
