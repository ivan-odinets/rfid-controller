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

#include "InputDeviceSelectorMenu.h"

#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QStyle>

InputDeviceSelectorMenu::InputDeviceSelectorMenu(QWidget* parent)
    : QMenu{parent}
{
    _setupUi();
    setToolTipsVisible(true);
    allowMultipleSelection(false);
}

InputDeviceSelectorMenu::InputDeviceSelectorMenu(const QString& title,QWidget* parent)
    : QMenu(title,parent)
{
    _setupUi();
    setToolTipsVisible(true);
    allowMultipleSelection(false);
}

InputDeviceSelectorMenu::~InputDeviceSelectorMenu()
{}

void InputDeviceSelectorMenu::allowMultipleSelection(bool arg)
{
    w_deviceActionsGroup->setExclusionPolicy((arg) ? QActionGroup::ExclusionPolicy::None : QActionGroup::ExclusionPolicy::ExclusiveOptional);
}

void InputDeviceSelectorMenu::setCurrentInputDeviceList(const InputDeviceInfoList& deviceList)
{
    for (const InputDeviceInfo& deviceInfo : deviceList)
        _addDeviceActionEntry(deviceInfo);
}

void InputDeviceSelectorMenu::clearDeviceList()
{
    foreach (QAction* action, w_deviceActionsGroup->actions()) {
        w_deviceActionsGroup->removeAction(action);
        this->removeAction(action);
        action->deleteLater();
    }
}

void InputDeviceSelectorMenu::deviceWasAttached(const InputDeviceInfo& details)
{
    _addDeviceActionEntry(details);
}

void InputDeviceSelectorMenu::deviceWasDetached(const InputDeviceInfo& details)
{
    QAction* action = _findDeviceAction(details);
    if (action == nullptr) {
        qDebug() << "action == nullptr. This should have not happened...";
        return;
    }

    w_deviceActionsGroup->removeAction(action);
    removeAction(action);
    action->deleteLater();
}

void InputDeviceSelectorMenu::deviceWasOpened(const InputDeviceInfo& details)
{
    QAction* action = _findDeviceAction(details);
    if (action != nullptr) {
        action->setChecked(true);
    } else {
        _addDeviceActionEntry(details,true);
    }
}

void InputDeviceSelectorMenu::deviceWasClosed(const InputDeviceInfo& details)
{
    QAction* action = _findDeviceAction(details);
    if (action == nullptr) {
        qDebug() << "action == nullptr. This should have not happened...";
        return;
    }

    action->setChecked(false);
}

void InputDeviceSelectorMenu::_deviceActionWasToggled(bool state)
{
    QAction* senderAction = qobject_cast<QAction*>(sender());
    Q_ASSERT(senderAction != nullptr);

    InputDeviceInfo deviceDetails = senderAction->data().value<InputDeviceInfo>();
    if (state) {
        emit deviceOpeningRequested(deviceDetails);
    } else {
        emit deviceClosureRequested(deviceDetails);
    }
}

void InputDeviceSelectorMenu::_setupUi()
{
    QAction* refreshAction = addAction(tr("Update device list"));
    refreshAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    connect(refreshAction,&QAction::triggered,this,&InputDeviceSelectorMenu::updateRequested);

    w_separatorAction = addSeparator();

    w_deviceActionsGroup = new QActionGroup(this);
}

QAction* InputDeviceSelectorMenu::_findDeviceAction(const InputDeviceInfo& details)
{
    foreach (QAction* action, w_deviceActionsGroup->actions())
        if (action->data().value<InputDeviceInfo>() == details)
            return action;

    return nullptr;
}

void InputDeviceSelectorMenu::_addDeviceActionEntry(const InputDeviceInfo &details, bool isConnected)
{
    if (_findDeviceAction(details) != nullptr)
        return;

    QAction* deviceAction = new QAction();

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    deviceAction->setText(details.deviceFileName());
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")#if defined(Q_OS_LINUX)
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC

    deviceAction->setData(QVariant::fromValue(details));
    QString actionToolTip = tr("VendorId=%1 ProductId=%2\r\nName: %3")
            .arg(QString::number(details.vendorId(),16))
            .arg(QString::number(details.productId(),16))
            .arg(details.deviceName());

    deviceAction->setToolTip(actionToolTip);
    deviceAction->setCheckable(true);
    deviceAction->setChecked(isConnected);
    connect(deviceAction,&QAction::triggered,this,&InputDeviceSelectorMenu::_deviceActionWasToggled);

    addAction(deviceAction);
    w_deviceActionsGroup->addAction(deviceAction);
}
