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

#include "NfcManager.h"

NfcManager::NfcManager(QObject *parent)
    : QObject{parent}
{}

void NfcManager::start()
{
    m_nfcManager.startTargetDetection();
}

void NfcManager::stop()
{
    m_nfcManager.stopTargetDetection();
}

void NfcManager::_nfcTargetDetected(QNearFieldTarget* target)
{
    emit keyFound(target->uid());

    target->deleteLater();
}
