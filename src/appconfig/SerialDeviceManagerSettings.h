#ifndef SERIALDEVICEMANAGERSETTINGS_H
#define SERIALDEVICEMANAGERSETTINGS_H

#include "./SettingsCore.h"

#include <QSerialPort>

#include "./core/serial/SerialPortConfig.h"
#include "./core/serial/SerialPortFilter.h"

class SerialDeviceManagerSettings : public virtual SettingsCore
{
public:
    static SerialDeviceManagerSettings* get() {
        Q_ASSERT(theOne != nullptr);
        return theOne;
    }

    void loadValues();

    SerialPortConfig    defaultSerialPortConfiguration() const { return m_serialPortConfig; }
    void                setDefaultSerialPortCongiguration(const SerialPortConfig& params);

    bool                serialDeviceAutoconnection() const { return m_serialDeviceAutoconnection; }
    void                setSerialDeviceAutoconnection(bool state);

    SerialPortFilter    serialDeviceFilter() const { return m_serialPortFilter; }
    void                saveSerialDeviceFilter(const SerialPortFilter& filter);
    void                appendSerialDeviceFilter(const SerialPortFilter& filter);

protected:
    SerialDeviceManagerSettings();

private:
    static SerialDeviceManagerSettings* theOne;

    SerialPortConfig    m_serialPortConfig;
    bool                m_serialDeviceAutoconnection;
    SerialPortFilter    m_serialPortFilter;
};

#endif // SERIALDEVICEMANAGERSETTINGS_H
