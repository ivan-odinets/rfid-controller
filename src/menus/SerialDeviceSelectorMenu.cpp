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

#include "SerialDeviceSelectorMenu.h"

#include <QActionGroup>
#include <QDebug>
#include <QSerialPortInfo>

SerialDeviceSelectorMenu::SerialDeviceSelectorMenu(QWidget* parent) :
    QMenu(parent),w_deviceActionsGroup(nullptr)
{
    _setupUi();
    setToolTipsVisible(true);
}

SerialDeviceSelectorMenu::SerialDeviceSelectorMenu(const QString& title,QWidget* parent) :
    QMenu(title,parent)
{
    _setupUi();
    setToolTipsVisible(true);
}

SerialDeviceSelectorMenu::~SerialDeviceSelectorMenu()
{}

void SerialDeviceSelectorMenu::allowMultipleSelection(bool arg)
{
    w_deviceActionsGroup->setExclusive(!arg);
}

void SerialDeviceSelectorMenu::setCurrentSerialDeviceList(const QList<QSerialPortInfo>& portList)
{
    for (const QSerialPortInfo& portInfo : portList)
        _addDeviceActionEntry(portInfo);
}

void SerialDeviceSelectorMenu::clearDeviceList()
{
    foreach (QAction* action, w_deviceActionsGroup->actions()) {
        w_deviceActionsGroup->removeAction(action);
        this->removeAction(action);
        action->deleteLater();
    }
}

void SerialDeviceSelectorMenu::deviceWasAttached(const QSerialPortInfo& details)
{
    _addDeviceActionEntry(details);
}

void SerialDeviceSelectorMenu::deviceWasDetached(const QSerialPortInfo& details)
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

void SerialDeviceSelectorMenu::deviceWasOpened(const QSerialPortInfo& details)
{
    QAction* action = _findDeviceAction(details);
    if (action != nullptr) {
        action->setChecked(true);
    } else {
        _addDeviceActionEntry(details,true);
    }
}

void SerialDeviceSelectorMenu::deviceWasClosed(const QSerialPortInfo& details)
{
    QAction* action = _findDeviceAction(details);
    if (action == nullptr) {
        qDebug() << "action == nullptr. This should have not happened...";
        return;
    }

    action->setChecked(false);
}

void SerialDeviceSelectorMenu::_deviceActionWasToggled(bool state)
{
    QAction* senderAction = qobject_cast<QAction*>(sender());
    Q_ASSERT(senderAction != nullptr);

    QSerialPortInfo deviceDetails = senderAction->data().value<QSerialPortInfo>();
    if (state) {
        emit deviceOpeningRequested(deviceDetails);
    } else {
        emit deviceClosureRequested(deviceDetails);
    }
}

void SerialDeviceSelectorMenu::_setupUi()
{
    QAction* refreshAction = addAction(tr("Update device list"));
    connect(refreshAction,&QAction::triggered,this,&SerialDeviceSelectorMenu::updateRequested);

    w_separatorAction = addSeparator();

    w_deviceActionsGroup = new QActionGroup(this);
}

QAction* SerialDeviceSelectorMenu::_findDeviceAction(const QSerialPortInfo& details)
{
    foreach (QAction* action, w_deviceActionsGroup->actions())
        if (action->data().value<QSerialPortInfo>().portName() == details.portName())
            return action;

    return nullptr;
}

void SerialDeviceSelectorMenu::_addDeviceActionEntry(const QSerialPortInfo &details, bool isConnected)
{
    QAction* deviceAction = new QAction();

    deviceAction->setData(QVariant::fromValue(details));
    QString actionToolTip = tr("VendorId=%1 ProductId=%2\r\nName: %3")
            .arg(QString::number(details.vendorIdentifier(),16))
            .arg(QString::number(details.productIdentifier(),16))
            .arg(details.portName());

    deviceAction->setToolTip(actionToolTip);
    deviceAction->setText(details.portName());
    deviceAction->setCheckable(true);
    deviceAction->setChecked(isConnected);
    connect(deviceAction,&QAction::triggered,this,&SerialDeviceSelectorMenu::_deviceActionWasToggled);

    addAction(deviceAction);
    w_deviceActionsGroup->addAction(deviceAction);
}
