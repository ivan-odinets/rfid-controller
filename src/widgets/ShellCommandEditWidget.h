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

#ifndef SHELLCOMMANDEDITWIDGET_H
#define SHELLCOMMANDEDITWIDGET_H

#include "CommandEditWidget.h"

class QCheckBox;
class QLineEdit;

class ShellCommand;

/*!
 *  @class ShellCommandEditWidget widgets/ShellCommandEditWidget.h
 *  @brief This class represents widget to display and edit ShellCommand objects
 */

class ShellCommandEditWidget : public CommandEditWidget
{
    Q_OBJECT
public:
    explicit ShellCommandEditWidget(QWidget *parent = nullptr);
    ~ShellCommandEditWidget() {}

protected:
    /*! @brief Implementing pure virtual method to display command. Required by CommandEditWidget */
    void _displayCommand(Command* command);

private slots:
    void _commandEnabledChecked(bool state);
    void _keyEditingFinished();
    void _programEditingFinished();
    void _argumentsEditingFinished();

private:
    inline void _setupUi();
    QCheckBox*     w_enabledCheckBox;
    QLineEdit*     w_keyEdit;
    QLineEdit*     w_programEdit;
    QLineEdit*     w_argumentListEdit;
};

#endif // SHELLCOMMANDEDITWIDGET_H
