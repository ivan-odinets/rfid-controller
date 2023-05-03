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

#include "CommandEditWidget.h"

#include "ShellCommandEditWidget.h"

CommandEditWidget::CommandEditWidget(QWidget *parent)
    : QWidget{parent}
{}

void CommandEditWidget::setCurrentCommand(Command* command)
{
    p_currentCommand = command;

    _displayCommand(command);
}

CommandEditWidget* CommandEditWidget::create(Command::Type cmdType)
{
    switch (cmdType) {
    case Command::Shell:
        return new ShellCommandEditWidget;
    case Command::Unknown:
        return nullptr;
    }
    Q_ASSERT(false);
    return nullptr;
}
