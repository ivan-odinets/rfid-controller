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

#ifndef INPUTDEVICEINFO_H
#define INPUTDEVICEINFO_H

#include <QDebug>
#include <QDir>
#include <QList>
#include <QTextStream>
#include <QString>

#include "./core/ServiceTypes.h"

/*!
 * @class InputDeviceInfo core/input/InputDeviceInfo.h
 * @brief This class holds information about InputDevice like vendorId, productId and so on.
 */

class InputDeviceInfo {
public:
    InputDeviceInfo()
        : m_vendorId(0x0000), m_productId(0x0000) {}
    ~InputDeviceInfo() {}

    static QList<InputDeviceInfo> availableInputDevices();

    bool       isValid() const;

    bool       hasVendorIdetifier() const                  { return m_vendorId != 0x0000; }
    VendorId   vendorId() const                            { return m_vendorId; }

    bool       hasProductIdentifier() const                { return m_productId != 0x0000; }
    ProductId  productId() const                           { return m_productId; }

    /*! @brief This method returns device name. Has nothind to do with device file name (linux) */
    QString    deviceName() const                          { return m_deviceName; }

private:
    VendorId   m_vendorId;
    ProductId  m_productId;
    QString    m_deviceName;

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of InputDeviceWatcher
//

public:
    /*! @brief This method returns InputDeviceInfo object for /dev/input/<name> file. Works only in Linux */
    static InputDeviceInfo fromDeviceFileName(const QString& devInputFileName);

    /*! @brief This method returns /dev/input/<device_name> path for input device. Works only in Linux */
    QString deviceFilePath() const                         { return m_deviceFilePath; }

    /*! @brief This method returns file name for this input device. Works only in Linux */
    QString deviceFileName() const                         { return m_deviceFileName; }

    /*! @brief This method returns /dev/input directory */
    static QDir inputDeviceDirectory()                     { return m_devInputDir; }

private:
    QString m_deviceFilePath;
    QString m_deviceFileName;

    static QDir              m_devInputDir;
    static QByteArray        _readSysFile(const QString& fileName);
    static bool              _hasVendorId(const QString& entry);
    static VendorId          _readVendorId(const QString& entry);
    static bool              _hasProductId(const QString& entry);
    static ProductId         _readProductId(const QString& entry);
    static QString           _readDeviceName(const QString& entry);

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")#if defined(Q_OS_LINUX)
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
};

Q_DECLARE_METATYPE(InputDeviceInfo)
typedef QList<InputDeviceInfo> InputDeviceInfoList;

QDebug        operator<<(QDebug dbg, const InputDeviceInfo& details);
QTextStream   &operator<<(QTextStream &out, const InputDeviceInfo& details);
bool          operator==(const InputDeviceInfo& lhs, const InputDeviceInfo& rhs);

#endif // INPUTDEVICEINFO_H
