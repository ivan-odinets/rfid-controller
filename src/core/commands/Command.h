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

#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

#include <QJsonObject>

/*!
 *  @class Command core/commands/Command.h
 *  @brief This class provides general interface to commands, which can be executed by this program
 */

class Command : public QObject
{
    Q_OBJECT
public:
    /*! @brief This enum holds information about currently supported command types */
    enum Type {
        Shell,     /*!< @brief Shell command, to launch applications and execute scripts */
        Unknown    /*!< @brief Type of command can not be determined */
    };

    virtual ~Command() {}

    /*! @brief Returns type of Command object in form of value from enum Command::Type. Must be implemented in
     *         Command subclasses */
    virtual Type   type() const = 0;

    void      run();

    QString   key() const                                  { return m_key; }
    void      setKey(const QString& key);

    void      setEnabled(bool state);
    bool      isEnabled() const                            { return m_enabled; }

    /*! @brief This method is used to create empty instances of Command subclasses objects */
    static Command*     create(Type);

    /*! @brief This method uis used to create Command objects from JSON input (used for loading/saving commands
     *         in files */
    static Command*     fromJson(const QJsonObject& json);

    /*! @brief Serialize this Command object to JSON. Should be implemented in Command subclasses */
    virtual QJsonObject toJson() const = 0;

    /*! @brief This method is used to cast this object to one of its subclasses */
    template<class C>
    C* to() { return dynamic_cast<C*>(this); }

signals:
    /*! @breif This signal is emitted when state ob this Command object has changed */
    void commandChanged();

protected:
    explicit Command(QObject* parent = nullptr);
    explicit Command(const QJsonObject& jsonObject,QObject* parent = nullptr);
    virtual void execute() = 0;

private:
    Q_DISABLE_COPY(Command);
    bool      m_enabled;
    QString   m_key;
};
Q_DECLARE_METATYPE(Command*)

#endif // COMMAND_H
