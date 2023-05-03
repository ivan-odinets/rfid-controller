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

#include "MonitorWidget.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget(parent)
{
    _setupUi();
}
MonitorWidget::~MonitorWidget()
{}

void MonitorWidget::displayLastKey(const QString& key)
{
    w_lastKeyRead->setText(key);
}

void MonitorWidget::setCommandList(CommandList* commandList)
{
    w_commandListWidget->setModel(commandList);
}

void MonitorWidget::changeEvent(QEvent *event)
{
    //Otherwise - setWindowModified(bool state) is not passed to parent
    //At least on Qt 5.12.2
    if (event->type() == QEvent::ModifiedChange)
        parentWidget()->setWindowModified(this->isWindowModified());

    QWidget::changeEvent(event);
}

void MonitorWidget::_setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    w_commandListWidget = new CommandListWidget;
    mainLayout->addWidget(w_commandListWidget);

    QHBoxLayout* lastKeyLayout = new QHBoxLayout;
    lastKeyLayout->addWidget(new QLabel(tr("Last key:")));
    w_lastKeyRead = new QLineEdit;
    w_lastKeyRead->setReadOnly(true);
    lastKeyLayout->addWidget(w_lastKeyRead);
    mainLayout->addLayout(lastKeyLayout);

    setLayout(mainLayout);
}
