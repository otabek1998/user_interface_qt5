# HMI


## User interface for Embedded systems 
User interface for Embedded system, by default project is made for screen resolution 800x480

## Description
This application applies simple UI for small screens. In this project I used Qt, libinput and Gstreamer frameworks.

## Installation
Primarily you should use Yocto project for installation. You should add new recipe with contents. Then after root filesystem is built you will get binary in /usr/bin directory on your target

```
SUMMARY = "Application Qt5 demo using Qt5"
AUTHOR = "Otabek Nazrullaev"
DESCRIPTION = " "

LICENSE = "CLOSED"
FILESEXTRAPATHS_prepend := "${THISDIR}/*"
FILESEXTRAPATHS_prepend_${PN} := "${THISDIR}/*"

inherit qmake5 pkgconfig

DEPENDS += "qtbase qtdeclarative libinput gstreamer"
RDEPENDS_${PN} = "audio-manager dlt-daemon dbus libinput gstreamer"

SRC_URI = "git://git@github.com:otabek1998/user_interface_qt5.git;protocol=ssh;branch=main"
SRCREV = '${AUTOREV}'

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

do_install_append () {
        install -m 0755 -d ${D}${bindir}
        install -m 0755 ${B}/qt-porting ${D}${bindir}
}

```

