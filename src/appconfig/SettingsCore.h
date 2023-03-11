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

#ifndef SETTINGSCORE_H
#define SETTINGSCORE_H

#include <QString>
#include <QVariant>

class QSettings;

class SettingsCore
{
public:
    virtual ~SettingsCore();

    void        setConfigFileName(const QString& fileName);
    QString     confiFileName() const;

    void        setPreserveConfig(bool arg) { m_preserveConfig = arg; }
    bool        preserveConfig() const      { return m_preserveConfig; }

protected:
    SettingsCore();
    bool        _initialized();
    QVariant    _value(const QString& key,const QVariant& defaultValue = QVariant()) const;
    void        _setValue(const QString& key,const QVariant& value);

    bool        _contains(const QString& key) const;

private:
    QSettings*  p_settings;
    bool        m_preserveConfig;
    QString     m_configFileName;
};

#endif // SETTINGSCORE_H
