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

#ifndef NFCMANAGER_H
#define NFCMANAGER_H

#include <QObject>

#include <QNearFieldManager>
#include <QNearFieldTarget>

class NfcManager : public QObject
{
    Q_OBJECT
public:
    explicit NfcManager(QObject *parent = nullptr);
    ~NfcManager() {}

    bool isNfcSupported() { return m_nfcManager.isSupported(); }

    void start();
    void stop();

signals:
    void keyFound(const QString& key);
    void errorMessage(const QString& errorMessage);

private slots:
    void _nfcTargetDetected(QNearFieldTarget* target);

private:
    QNearFieldManager m_nfcManager;
};

#endif // NFCMANAGER_H
