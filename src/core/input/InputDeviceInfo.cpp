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

#include "InputDeviceInfo.h"

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    #include <QRegularExpression>
#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific code
//

QDir InputDeviceInfo::m_devInputDir("/dev/input");

QList<InputDeviceInfo> InputDeviceInfo::availableInputDevices()
{
    QStringList devInputFileList = m_devInputDir.entryList(QDir::System | QDir::NoDotAndDotDot);
    InputDeviceInfoList result;

    //Search for attached devices
    for (int i = 0; i < devInputFileList.count(); i++)
        result.append(InputDeviceInfo::fromDeviceFileName(devInputFileList.at(i)));

    return result;
}

bool InputDeviceInfo::isValid() const
{
    return QFile::exists(m_deviceFilePath);
}

InputDeviceInfo InputDeviceInfo::fromDeviceFileName(const QString& devInputFileName)
{
    //Function is checking /sys/class/input/ subdirectories in order to get information about /dev/input/ devices
    //Possible because device files names in /dev/input are the same as names of directories in /sys/class/input

    InputDeviceInfo result;

    result.m_deviceFileName = devInputFileName;
    result.m_vendorId  = _readVendorId(devInputFileName);

    result.m_productId = _readProductId(devInputFileName);

    result.m_deviceFilePath = QString("/dev/input/%1").arg(devInputFileName);
    result.m_deviceName = _readDeviceName(devInputFileName);

    return result;
}

bool InputDeviceInfo::_hasVendorId(const QString& entry)
{
    return QFile::exists(QString("/sys/class/input/%1/device/id/vendor").arg(entry));
}

quint16 InputDeviceInfo::_readVendorId(const QString& entry)
{
    return _readSysFile(QString("/sys/class/input/%1/device/id/vendor").arg(entry)).toUInt(nullptr,16);
}

bool InputDeviceInfo::_hasProductId(const QString& entry)
{
    return QFile::exists(QString("/sys/class/input/%1/device/id/product").arg(entry));
}

quint16 InputDeviceInfo::_readProductId(const QString& entry)
{
    return _readSysFile(QString("/sys/class/input/%1/device/id/product").arg(entry)).toUInt(nullptr,16);
}

QString InputDeviceInfo::_readDeviceName(const QString& entry)
{
    return QString(_readSysFile(QString("/sys/class/input/%1/device/name").arg(entry))).remove(QRegularExpression{R"-((\r\n?|\n))-"});
}

QByteArray InputDeviceInfo::_readSysFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QByteArray();
    }

    QByteArray result = file.readAll();
    file.close();
    return result;
}

bool operator==(const InputDeviceInfo& lhs, const InputDeviceInfo& rhs)
{
    return  (lhs.vendorId()             == rhs.vendorId())        &&
            (lhs.productId()            == rhs.productId())       &&
            (lhs.deviceFileName()       == rhs.deviceFileName())  &&
            (lhs.deviceFilePath()       == rhs.deviceFilePath());
}

QDebug operator<<(QDebug dbg, const InputDeviceInfo& details)
{
    dbg.nospace() << qUtf8Printable(QString("InputDeviceInfo(vendorId=%1; productId=%2; deviceFile=%3; deviceName=%4)")
                                    .arg(QString::number(details.vendorId(),16))
                                    .arg(QString::number(details.productId(),16))
                                    .arg(details.deviceFileName())
                                    .arg(details.deviceName()));

    return dbg.maybeSpace();
}

QTextStream &operator<<(QTextStream &out, const InputDeviceInfo& details)
{
    out << qUtf8Printable(QString("(vendorId=%1; productId=%2; deviceFile=%3; deviceName=%4)")
                          .arg(QString::number(details.vendorId(),16))
                          .arg(QString::number(details.productId(),16))
                          .arg(details.deviceFileName())
                          .arg(details.deviceName()));
    return out;
}

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
