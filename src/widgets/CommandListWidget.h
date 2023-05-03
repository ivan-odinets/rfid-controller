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

#ifndef COMMANDLISTWIDGET_H
#define COMMANDLISTWIDGET_H

#include <QWidget>

#include <QList>

class QScrollArea;
class QVBoxLayout;

class Command;
class CommandList;
class CommandEditWidget;

/*!
 *  @class CommandListWidget widgets/CommandListWidget.h
 *  @brief This widget is used to display CommandList object as well as allow user to edit and interact with its data
 */

class CommandListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommandListWidget(QWidget* parent = nullptr);
    ~CommandListWidget() {}

    /*! @brief This method is used to display CommandList */
    void setModel(CommandList* model);

public slots:
    /*! @brief This slot is uset to clear all commands from current CommandList */
    void clear();

protected:
    /*! @brief Reimplementing this method to allow user create new commands by double-click */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void _commandAdditionRequested();

    /*! @brief This slot is invoked when Command was added to the currently displayed CommandList */
    void _commandAdded(Command* comm1and);

    /*! @brief This slot is invoked when user requests to remove one of the active commands */
    void _commandRemovalRequested();

    /*! @brief This slot should be invoked when Command was removed from the currently displayed CommandList */
    void _commandRemoved(Command* command);

    /*! @brief This slot should be invoked when the currently displayed CommandList was changed */
    void _commandListChanged();

    void _contextMenuRequested(const QPoint& point);

private:
    /*! @brief This method just removes all displayed widgets */
    void _clearView();

    inline void _setupUi();
    CommandList*                  p_currentCommandList;
    QWidget*                      w_commandsContainerWidget;
    QVBoxLayout*                  w_scrollableLayout;
    QList<CommandEditWidget*>     m_widgetList;
    QScrollArea*                  w_commandsScrollArea;

};

#endif // COMMANDLISTWIDGET_H
