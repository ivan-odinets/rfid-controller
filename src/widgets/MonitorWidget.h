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

#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>

#include "CommandListWidget.h"
#include "core/commands/CommandList.h"

class QEvent;
class QLabel;
class QLineEdit;

class ShellCommandEditWidget;

/*!
 *  @class MonitorWidget widgets/MonitorWidget.h
 *  @brief This is the class for central widget. Can display last readed key and contains CommandListWidget object
 *         for interacting with CommandList
 */

class MonitorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MonitorWidget(QWidget* parent = 0);
    ~MonitorWidget();

    void setCommandList(CommandList* commandList);

protected:
    void changeEvent(QEvent* event) override;

public slots:
    /*! @brief This method is invoked when a key was readed by any of the connected devices. */
    void displayLastKey(const QString& key);

private:
    inline void _setupUi();
    QLineEdit*               w_lastKeyRead;
    CommandListWidget*       w_commandListWidget;
};

#endif // MONITORWIDGET_H
