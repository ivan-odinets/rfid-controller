#
# App build options
#

DEFINES  += APP_NAME='\\"RFID\ Controller\\"'
DEFINES  += APP_VERSION='\\"0.1.1\\"'

#Comment this disable support of HID devices (e.g. USB based rfid readers)
DEFINES  += HID

#Comment this to disable support of serial devices (e.g. DIY arduino-based readers)
DEFINES  += SERIAL

#Comment this to disable support of NFC
#DEFINES  += NFC

#Uncomment this to enable GUI support
DEFINES  += GUI

#Comment this to enable support of systemtray
#DEFINES  += QT_NO_SYSTEMTRAYICON

CONFIG(release, debug|release) : DEFINES += QT_NO_DEBUG_OUTPUT

TARGET   = rfid-controller
CONFIG   += c++17
QT       += core

DESTDIR            = ../bin
MOC_DIR            = ../build/moc
RCC_DIR            = ../build/rcc
unix:OBJECTS_DIR   = ../build/o/unix
win32:OBJECTS_DIR  = ../build/o/win32

#
# Add all needed *.h and *.cpp files
#

HEADERS += \
    appconfig/CommandLineParser.h \
    appconfig/RfidControllerSettings.h \
    appconfig/Settings.h \
    appconfig/SettingsCore.h \
    core/CommandListManager.h \
    core/NotificationType.h \
    core/RfidController.h \
    core/ServiceTypes.h \
    core/commands/Command.h \
    core/commands/CommandList.h \
    core/commands/ShellCommand.h

SOURCES += \
    appconfig/CommandLineParser.cpp \
    appconfig/RfidControllerSettings.cpp \
    appconfig/Settings.cpp \
    appconfig/SettingsCore.cpp \
    core/CommandListManager.cpp \
    core/NotificationType.cpp \
    core/RfidController.cpp \
    core/ServiceTypes.cpp \
    core/commands/Command.cpp \
    core/commands/CommandList.cpp \
    core/commands/ShellCommand.cpp \
    main.cpp

DISTFILES += \
    LICENSE \
    README.md

RESOURCES += \
    resources.qrc

#
# If we have HID support enabled - we need these files
#

contains(DEFINES, HID) {
    HEADERS += \
        appconfig/InputDeviceManagerSettings.h \
        core/devices/InputDevice.h \
        core/input/InputDeviceFilter.h \
        core/input/InputDeviceInfo.h \
        core/input/InputDeviceManager.h \
        core/input/InputDeviceWatcher.h \
        core/input/InputEvent.h

    SOURCES += \
        appconfig/InputDeviceManagerSettings.cpp \
        core/devices/InputDevice.cpp \
        core/input/InputDeviceFilter.cpp \
        core/input/InputDeviceInfo.cpp \
        core/input/InputDeviceManager.cpp \
        core/input/InputDeviceWatcher.cpp \
        core/input/InputEvent.cpp
}

#
# If we have Serial support enabled - we need these files
#

contains(DEFINES, SERIAL) {
    QT += serialport

    HEADERS += \
        appconfig/SerialDeviceManagerSettings.h \
        core/devices/SerialDevice.h \
        core/serial/SerialPortConfig.h \
        core/serial/SerialPortFilter.h \
        core/serial/SerialDeviceManager.h \
        core/serial/SerialDeviceWatcher.h

    SOURCES += \
        appconfig/SerialDeviceManagerSettings.cpp \
        core/devices/SerialDevice.cpp \
        core/serial/SerialPortConfig.cpp \
        core/serial/SerialPortFilter.cpp \
        core/serial/SerialDeviceManager.cpp \
        core/serial/SerialDeviceWatcher.cpp
}

#
# If we have NFC support enabled - we need these files
#

contains(DEFINES, NFC) {
    QT += nfc

    HEADERS += \
        core/nfc/NfcManager.h

    SOURCES += \
        core/nfc/NfcManager.cpp
}

#
# If we have GUI support enabled - we need these files
#

contains(DEFINES, GUI) {
    QT       += gui widgets

    HEADERS += \
        appconfig/MainWindowSettings.h \
        menus/NotificationTypeMenu.h \
        widgets/Hex16BitSpinBox.h \
        widgets/MonitorWidget.h \
        widgets/CommandEditWidget.h \
        widgets/CommandListWidget.h \
        widgets/ShellCommandEditWidget.h \
        windows/MainWindow.h

    SOURCES += \
        appconfig/MainWindowSettings.cpp \
        menus/NotificationTypeMenu.cpp \
        widgets/Hex16BitSpinBox.cpp \
        widgets/MonitorWidget.cpp \
        widgets/CommandEditWidget.cpp \
        widgets/CommandListWidget.cpp \
        widgets/ShellCommandEditWidget.cpp \
        windows/MainWindow.cpp

    # Some HID-related GUI files
    contains(DEFINES, HID) {
        HEADERS += \
            menus/InputDeviceSelectorMenu.h \
            windows/InputFilterEditDialog.h

        SOURCES += \
            menus/InputDeviceSelectorMenu.cpp \
            windows/InputFilterEditDialog.cpp
    } # GUI + HID

    # Some Serial-related GUI files
    contains(DEFINES, SERIAL) {
        HEADERS += \
            menus/SerialDeviceSelectorMenu.h \
            windows/SerialFilterEditDialog.h \
            windows/SerialPortConfiguratorDialog.h

        SOURCES += \
            menus/SerialDeviceSelectorMenu.cpp \
            windows/SerialFilterEditDialog.cpp \
            windows/SerialPortConfiguratorDialog.cpp
    } # GUI + SERIAL
} #GUI
