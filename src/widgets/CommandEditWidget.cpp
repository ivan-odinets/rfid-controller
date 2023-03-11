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

#include "CommandEditWidget.h"

#include "ShellCommandEditWidget.h"

CommandEditWidget::CommandEditWidget(QWidget *parent)
    : QWidget{parent}
{}

void CommandEditWidget::setCurrentCommand(Command* command)
{
    p_currentCommand = command;

    displayCommand(command);
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
