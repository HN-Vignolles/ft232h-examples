TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
        src/fast.c \
        src/mpsse.c \
        src/support.c \
        ssd1306_ftdi.c

HEADERS += \
    font5x7.h \
    font7x10.h \
    src/mpsse.h \
    src/support.h \
    ssd1306_ftdi.h

unix|win32: LIBS += -lftdi
