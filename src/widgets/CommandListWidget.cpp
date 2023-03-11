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

#include "CommandListWidget.h"

#include <QMenu>
#include <QVBoxLayout>
#include <QScrollArea>

#include "CommandEditWidget.h"
#include "core/commands/CommandList.h"

CommandListWidget::CommandListWidget(QWidget* parent)
    : QWidget(parent),p_currentCommandList(nullptr)
{
    _setupUi();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&CommandListWidget::customContextMenuRequested,this,&CommandListWidget::_contextMenuRequested);

    setToolTip(tr("Double click to add new empty command"));
}

void CommandListWidget::setModel(CommandList* model)
{
    if (p_currentCommandList != nullptr) {
        disconnect(p_currentCommandList,&CommandList::commandRemoved,this,&CommandListWidget::_commandRemoved);
        disconnect(p_currentCommandList,&CommandList::commandAdded,this,&CommandListWidget::_commandAdded);
        disconnect(p_currentCommandList,&CommandList::commandListChanged,this,&CommandListWidget::_commandListChanged);
        _clearView();
    }

    p_currentCommandList = model;
    if (p_currentCommandList == nullptr)
        return;

    connect(p_currentCommandList,&CommandList::commandRemoved,this,&CommandListWidget::_commandRemoved);
    connect(p_currentCommandList,&CommandList::commandAdded,this,&CommandListWidget::_commandAdded);
    connect(p_currentCommandList,&CommandList::commandListChanged,this,&CommandListWidget::_commandListChanged);

    for (int i = 0; i < p_currentCommandList->count(); i++) {
        _commandAdded(p_currentCommandList->at(i));
    }
}

void CommandListWidget::clear()
{
    if (p_currentCommandList != nullptr)
        p_currentCommandList->clear();
}

void CommandListWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    _commandAdditionRequested();

    QWidget::mouseDoubleClickEvent(event);
}

void CommandListWidget::_commandAdditionRequested()
{
    if (p_currentCommandList != nullptr)
        p_currentCommandList->append(Command::create(Command::Shell));

}

void CommandListWidget::_commandAdded(Command* command)
{
    CommandEditWidget* cmdWidget = CommandEditWidget::create(command->type());

    connect(cmdWidget,&CommandEditWidget::removeCurrentCommand,this,&CommandListWidget::_commandRemovalRequested);

    cmdWidget->setCurrentCommand(command);
    m_widgetList.append(cmdWidget);
    w_scrollableLayout->insertWidget(w_scrollableLayout->count()-1,cmdWidget); //w_scrollableLayout has stretch in the end.
                                                                               //So we need insert widget at one before last position
}

void CommandListWidget::_commandRemovalRequested()
{
    CommandEditWidget* senderWidget = qobject_cast<CommandEditWidget*>(sender());
    Q_ASSERT(senderWidget != nullptr);
    p_currentCommandList->removeCommand(senderWidget->currentCommand());
}

void CommandListWidget::_commandRemoved(Command* command)
{
    for (int i = 0; i < m_widgetList.count(); i++) {
        CommandEditWidget* current = m_widgetList.at(i);

        if (current->currentCommand() == command) {
            w_scrollableLayout->removeWidget(current);
            m_widgetList.removeAll(current);
            current->deleteLater();
            return;
        }
    }
}

void CommandListWidget::_commandListChanged()
{
    parentWidget()->setWindowModified(true);
}

void CommandListWidget::_contextMenuRequested(const QPoint& point)
{
    QMenu contextMenu;
    contextMenu.addAction(tr("Add command"),this,&CommandListWidget::_commandAdditionRequested);
    contextMenu.addAction(tr("Clear"),this,&CommandListWidget::clear);

    contextMenu.exec(mapToGlobal(point));
}

void CommandListWidget::_clearView()
{
    for (CommandEditWidget* widget : m_widgetList) {
        w_scrollableLayout->removeWidget(widget);
        widget->deleteLater();
    }

    m_widgetList.clear();
}

/*
 **********************************************************************************************************************
 * Setting up UI
 */

void CommandListWidget::_setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    w_commandsScrollArea = new QScrollArea;
    w_commandsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    w_commandsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    w_commandsScrollArea->setWidgetResizable(true);

    w_scrollableLayout = new QVBoxLayout;
    w_scrollableLayout->addStretch();
    w_commandsContainerWidget = new QWidget;
    w_commandsContainerWidget->setLayout(w_scrollableLayout);
    w_commandsScrollArea->setWidget(w_commandsContainerWidget);

    mainLayout->addWidget(w_commandsScrollArea);
    setLayout(mainLayout);
}
