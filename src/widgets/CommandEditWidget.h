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

#ifndef COMMANDEDITWIDGET_H
#define COMMANDEDITWIDGET_H

#include <QWidget>

#include "./core/commands/Command.h"

/*!
 *  @class CommandEditWidget widgets/CommandEditWidget.h
 *  @brief This class represents general interface for widgets, reponsible for viewing and editing commands
 */

class CommandEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommandEditWidget(QWidget *parent = nullptr);
    ~CommandEditWidget() {}

    void      setCurrentCommand(Command* command);
    Command*  currentCommand() const                  { return p_currentCommand; }

    /*! @brief This method is used to create objects of specific CommandEditWidget subclass depending on
     *         type of command this widget is going to work with */
    static CommandEditWidget* create(Command::Type cmdType);

signals:
    /*! @brief This signal is emitted when user asks to remove Command assosiated with this widget */
    void removeCurrentCommand();

protected:
    /*! @brief This method is implemented in subclasses of CommandEdit widget. Used to display actual command data */
    virtual void displayCommand(Command* command) = 0;

private:
    Command* p_currentCommand;
};

#endif // COMMANDEDITWIDGET_H
