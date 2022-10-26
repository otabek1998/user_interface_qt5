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
    src/touchinput.cpp \
    src/screensaver.cpp \
    src/customframe.cpp \
    src/framechanger.cpp

INVITE_LIBS += libinput
CONFIG += link_pkgconfig
PKGCONFIG += $$INVITE_LIBS

HEADERS  += include/homescreen.h \
    include/analogclock.h \
    include/touchinput.h \
    include/screensaver.h \
    include/customframe.h \
    include/framechanger.h

FORMS    += \
    ui/homescreen.ui \
    ui/screensaver.ui \
    ui/framechanger.ui

DISTFILES += \
    Bitmaps/home_screen/0569-bg_1_home.png

RESOURCES += \
    home_screen_images.qrc
