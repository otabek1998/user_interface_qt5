#-------------------------------------------------
#
# Project created by QtCreator 2022-09-07T13:51:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-porting
TEMPLATE = app


SOURCES += src/main.cpp\
        src/homescreen.cpp \
    src/analogclock.cpp \
    src/hw-input/InputHandler.cpp \
    src/hw-input/SignalConverter.cpp \
    src/hw-input/SignalTraits.cpp \
    src/hw-input/SoftkeyConverter.cpp

HEADERS  += include/homescreen.h \
    include/analogclock.h \
    src/hw-input/SignalConverter.hpp \
    src/hw-input/SignalTraits.hpp \
    src/hw-input/SoftkeyConverter.hpp \
    include/HWInput/Common.hpp \
    include/HWInput/ICallback.hpp \
    include/HWInput/InputHandler.hpp

FORMS    += \
    ui/homescreen.ui

DISTFILES += \
    Bitmaps/home_screen/0569-bg_1_home.png

RESOURCES += \
    home_screen_images.qrc
