SUBDIRS += src/RfidController.pro

TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release 

DISTFILES += \
    .gitignore \
    LICENSE \
    README.md \
    debian/rfid-controller.desktop \
    debian/rfid-controller.install \
    docs/Doxyfile

