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

#include "SerialDeviceWatcher.h"

SerialDeviceWatcher::SerialDeviceWatcher(QObject *parent)
    : QObject{parent},m_autoupdateInterval(500)
{}

SerialDeviceWatcher::~SerialDeviceWatcher()
{
    stop();
}

void SerialDeviceWatcher::start()
{
    updateDeviceList();
    m_autoupdateTimer.setInterval(m_autoupdateInterval);
    m_autoupdateTimer.start();
    connect(&m_autoupdateTimer,&QTimer::timeout,this,&SerialDeviceWatcher::updateDeviceList);
}

void SerialDeviceWatcher::stop()
{
    m_autoupdateTimer.stop();
}

void SerialDeviceWatcher::updateDeviceList()
{
    QList<QSerialPortInfo> newDeviceInfoList = QSerialPortInfo::availablePorts();
    auto contains = [](const QList<QSerialPortInfo>& list,const QSerialPortInfo& info) {
        for (int i = 0; i < list.count(); i++) {
            if (list.at(i).portName() == info.portName())
                return true;
        }
        return false;
    };

    //Search for newly connected devices
    for (int i = 0; i < newDeviceInfoList.count(); i++) {
        if (!contains(m_lastScanResults,newDeviceInfoList.at(i))) {
            emit deviceWasAttached(newDeviceInfoList.at(i));
        }
    }

    //Check for removed devices
    for (int i = 0; i < m_lastScanResults.count(); i++) {
        if (!contains(newDeviceInfoList,m_lastScanResults.at(i))) {
            emit deviceWasDetached(m_lastScanResults.at(i));
        }
    }

    //Save last scan results for further refresh() calls
    m_lastScanResults = newDeviceInfoList;
}

void SerialDeviceWatcher::setAutoupdateInterval(uint newInterval)
{
    m_autoupdateInterval = newInterval;
    if (m_autoupdateTimer.isActive()) {
        m_autoupdateTimer.stop();
        m_autoupdateTimer.start(m_autoupdateInterval);
    }
}
