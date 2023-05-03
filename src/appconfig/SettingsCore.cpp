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

#include "SettingsCore.h"

#include <QDebug>
#include <QSettings>

SettingsCore::SettingsCore()
    : p_settings(nullptr),m_preserveConfig(false)
{}

SettingsCore::~SettingsCore()
{
    p_settings->sync();
    delete p_settings;
}

void SettingsCore::setConfigFileName(const QString &fileName)
{
    m_configFileName = fileName;
}

bool SettingsCore::_initialized()
{
    Q_ASSERT(p_settings == nullptr);

    p_settings = (m_configFileName.isEmpty()) ? new QSettings()
                    : new QSettings(m_configFileName,QSettings::NativeFormat);

    return true;
}

QString SettingsCore::confiFileName() const
{
    Q_ASSERT(p_settings != nullptr);
    return p_settings->fileName();
}

QVariant SettingsCore::_value(const QString& key,const QVariant& defaultValue) const
{
    Q_ASSERT(p_settings != nullptr);
    return p_settings->value(key,defaultValue);
}

void SettingsCore::_setValue(const QString& key,const QVariant& value)
{
    if (m_preserveConfig)
        return;

    Q_ASSERT(p_settings != nullptr);
    p_settings->setValue(key,value);
}

bool SettingsCore::_contains(const QString& key) const
{
    Q_ASSERT(p_settings != nullptr);
    return p_settings->contains(key);
}
