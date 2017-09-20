TEMPLATE = app       # bynary file type (application)
TARGET   = TciClient # application name

#############################################################
#                        QT5 modules                        #
QT += core gui widgets network websockets
#############################################################

# target directory
macx {
    # Apple OS X
    CONFIG(release, debug|release): DESTDIR = bin/macx      # release
    CONFIG(debug  , debug|release): DESTDIR = bin/macx_d    # debug
}
unix:!macx {
    # Linux
    equals(QT_ARCH, arm) {
        CONFIG(release, debug|release): DESTDIR = bin/linuxARM   # release
        CONFIG(debug  , debug|release): DESTDIR = bin/linuxARM_d # debug
    }
    else {
        contains(QT_ARCH, i386) {
            # 32 bit
            CONFIG(release, debug|release): DESTDIR = bin/linux32   # release
            CONFIG(debug  , debug|release): DESTDIR = bin/linux32_d # debug
        }
        else {
            # 64 bit
            CONFIG(release, debug|release): DESTDIR = bin/linux64   # release
            CONFIG(debug  , debug|release): DESTDIR = bin/linux64_d # debug
        }
    }
}
win32 {
    # Windows
    contains(QT_ARCH, i386) {
        # 32 bit
        win32-msvc* {
            # Visual Studio
            CONFIG(release, debug|release): DESTDIR = bin/win32_msvc    # release
            CONFIG(debug  , debug|release): DESTDIR = bin/win32_msvc_d  # debug
        }
        win32-g++ {
            # MinGW
            CONFIG(release, debug|release): DESTDIR = bin/win32_mingw   # release
            CONFIG(debug  , debug|release): DESTDIR = bin/win32_mingw_d # debug
        }
    }
    else {
        # 64 bit
        win32-msvc* {
            # Visual Studio
            CONFIG(release, debug|release): DESTDIR = bin/win64_msvc    # release
            CONFIG(debug  , debug|release): DESTDIR = bin/win64_msvc_d  # debug
        }
        win32-g++ {
            # MinGW
            CONFIG(release, debug|release): DESTDIR = bin/win64_mingw   # release
            CONFIG(debug  , debug|release): DESTDIR = bin/win64_mingw_d # debug
        }
    }
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# target directory for temp files
MOC_DIR     = tmp
OBJECTS_DIR = tmp
RCC_DIR     = tmp
UI_DIR      = tmp

# enable С++11
CONFIG += c++11

#############################################################
#                Include source code files                  #
#############################################################
SOURCES += source/main.cpp \
    source/dsp/Window/Window.cpp \
    source/dsp/Spectrum/Spectrum.cpp \
    source/gui/MainWindow.cpp

HEADERS += source/gui/MainWindow.h \
    source/dsp/spectrumringbuffer.h \
    source/dsp/Window/Window.h \
    source/dsp/Spectrum/Spectrum.h \
    source/dsp/dsp.h

FORMS   += source/gui/MainWindow.ui

include(libs/TciClient/TciClient.pri)
include(libs/QCustomPlot/QCustomPlot.pri)

#############################################################
#                 Include resource files                    #
#############################################################
RESOURCES += resource/resource.qrc

