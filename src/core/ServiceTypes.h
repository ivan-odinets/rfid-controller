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

#ifndef SERVICETYPES_H
#define SERVICETYPES_H

#include <QSet>
#include <QString>

/*!
 * @class WordSet core/ServiceTypes.h
 * @brief Nothing more than QSet<quint16> with some extra-functions
 */

class WordSet : public QSet<quint16>
{
public:
    WordSet() : QSet<quint16>() {}
    WordSet(const QString& sourceString,const QChar& separator = QChar(','));
    QString toString(const QChar& separator = QChar(',')) const;
};

/*!
 * @class StringSet core/ServiceTypes.h
 * @brief Nothing more than QSet<QString> with some extra-functions
 */

class StringSet : public QSet<QString>
{
public:
    StringSet() : QSet<QString>() {}
    StringSet(const QString& sourceString,const QChar& separator = QChar(','));
    QString toString(const QChar& separator = QChar(',')) const;
};

typedef quint16         ProductId;
typedef WordSet         ProductIdSet;

typedef quint16         VendorId;
typedef WordSet         VendorIdSet;

typedef StringSet       SerialPortNamesSet;

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    typedef StringSet       DeviceNameSet;
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC

#endif // SERVICETYPES_H
