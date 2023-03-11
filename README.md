# RFID Controller

This application allow executing commands based on readings from devices connected to computer. It can be used together with any other devices, which are recognized as a keyboard and producing /dev/input/* events.

Icons used: [Rfid icons created by Freepik - Flaticon](https://www.flaticon.com/free-icons/rfid)

Version: 0.1.0

## Features

Currently working:
- Reading from HID devices. Can be used with usb-based RFID and NFC readers which are recognized as a virtual keyboard;
- Reading from serial devices. Can be used together with Arduino-based DIY RFID and NFC readers;
- Automatic connection of devices based on their VendorId and ProductId as well as on their names;
- Saving and restoring files with commands in JSON format;

Currently only Linux-based systems are supported. Tested with Qt 6.4.2 and Qt 5.15.8
In future support for Android and Windows systems will be added.

**Important: This is early release, so some functionality may be changed in future.**

## Compiling

For Debian-based distros
```bash
git clone https://github.com/ivan-odinets/rfid-controller.git
cd RfidController
qmake
debuild -b -uc -us
```

For other Linux distros
```bash
git clone https://github.com/ivan-odinets/rfid-controller.git
cd RfidController
qmake
make
cd bin
./rfid-controller -h
```

## Configuration and usage

Configuration can be done either using config file or using command line options. Default config file location - ~/.config/OdinSoft/RFID Controller.conf

By default explicitly specified command line options have prioriry over corresponding options from config file. Moreover, config file in this case will be updated.

## udev configuration (not neccesary, but can be done)

To use static naming of the devices based on their vendor id create udev rules file with the similar content. Change idVendor to match your device.

```
KERNEL!="event[0-9]*", GOTO="rfid_end"
ATTRS{idVendor}=="08ff", SYMLINK+="input/rfid", MODE="0666"
LABEL="rfid_end"
```

And after this:
```bash
sudo udevadm control --reload
```

# License
[GNU General Public License v3.0](https://choosealicense.com/licenses/gpl-3.0/)
