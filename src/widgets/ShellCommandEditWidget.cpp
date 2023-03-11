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

#include "ShellCommandEditWidget.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>

#include "core/commands/ShellCommand.h"

ShellCommandEditWidget::ShellCommandEditWidget(QWidget *parent)
    : CommandEditWidget{parent}
{
    _setupUi();
}

void ShellCommandEditWidget::displayCommand(Command* command)
{
    ShellCommand* shellCommand = command->to<ShellCommand>();
    Q_ASSERT(shellCommand != nullptr);

    w_enabledCheckBox->setChecked(shellCommand->isEnabled());
    w_keyEdit->setText(shellCommand->key());
    w_programEdit->setText(shellCommand->program());
    w_argumentListEdit->setText(shellCommand->arguments().join(" "));
}

void ShellCommandEditWidget::_commandEnabledChecked(bool state)
{
    if (currentCommand() == nullptr)
        return;

    ShellCommand* shellCommand = currentCommand()->to<ShellCommand>();
    Q_ASSERT(shellCommand != nullptr);

    if (shellCommand->isEnabled() != state)
        shellCommand->setEnabled(state);
}

void ShellCommandEditWidget::_keyEditingFinished()
{
    if (currentCommand() == nullptr)
        return;

    ShellCommand* shellCommand = currentCommand()->to<ShellCommand>();
    Q_ASSERT(shellCommand != nullptr);

    if (shellCommand->key() != w_keyEdit->text())
        shellCommand->setKey(w_keyEdit->text());
}

void ShellCommandEditWidget::_programEditingFinished()
{
    if (currentCommand() == nullptr)
        return;

    ShellCommand* shellCommand = currentCommand()->to<ShellCommand>();
    Q_ASSERT(shellCommand != nullptr);

    if (shellCommand->program() != w_programEdit->text())
        shellCommand->setProgram(w_programEdit->text());
}

void ShellCommandEditWidget::_argumentsEditingFinished()
{
    if (currentCommand() == nullptr)
        return;

    ShellCommand* shellCommand = currentCommand()->to<ShellCommand>();
    Q_ASSERT(shellCommand != nullptr);

    if (shellCommand->argumentsString() != w_argumentListEdit->text())
        shellCommand->setArguments(w_argumentListEdit->text().split(" "));
}

void ShellCommandEditWidget::_setupUi()
{
    QHBoxLayout* mainLayout = new QHBoxLayout;

    w_enabledCheckBox = new QCheckBox;
    w_enabledCheckBox->setToolTip(tr("Is command enabled?"));
    mainLayout->addWidget(w_enabledCheckBox);
    connect(w_enabledCheckBox,&QCheckBox::clicked,this,&ShellCommandEditWidget::_commandEnabledChecked);

    w_keyEdit = new QLineEdit;
    w_keyEdit->setToolTip(tr("Key, activating command"));
    mainLayout->addWidget(w_keyEdit);
    connect(w_keyEdit,&QLineEdit::editingFinished,this,&ShellCommandEditWidget::_keyEditingFinished);

    w_programEdit = new QLineEdit;
    w_programEdit->setToolTip(tr("Program to start"));
    mainLayout->addWidget(w_programEdit);
    connect(w_programEdit,&QLineEdit::editingFinished,this,&ShellCommandEditWidget::_programEditingFinished);

    w_argumentListEdit = new QLineEdit;
    w_argumentListEdit->setToolTip(tr("Command line arguments"));
    mainLayout->addWidget(w_argumentListEdit);
    connect(w_argumentListEdit,&QLineEdit::editingFinished,this,&ShellCommandEditWidget::_argumentsEditingFinished);

    QPushButton* deleteButton = new QPushButton;
    deleteButton->setIcon(style()->standardIcon(QStyle::SP_DialogAbortButton));
    deleteButton->setToolTip(tr("Delete this command"));
    connect(deleteButton,&QPushButton::clicked,this,&ShellCommandEditWidget::removeCurrentCommand);
    mainLayout->addWidget(deleteButton);

    setLayout(mainLayout);
}
