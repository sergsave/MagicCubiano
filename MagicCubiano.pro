#-------------------------------------------------
#
# Project created by QtCreator 2018-11-17T14:53:59
#
#-------------------------------------------------

CONFIG += c++14

QT       += core gui bluetooth multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicCubiano
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/View/EdgeSettingsWidget.cpp \
    src/View/MainWindow.cpp \
    src/GiikerProtocol.cpp \
    src/GuitarFretboardPos.cpp \
    src/main.cpp \
    src/SoundGenerator.cpp \
    src/View/ConnectionDialog.cpp

HEADERS += \
    src/View/EdgeSettingsWidget.h \
    src/View/MainWindow.h \
    src/GiikerProtocol.h \
    src/GuitarFretboardPos.h \
    src/MagicCubianoGlobal.h \
    src/SoundGenerator.h \
    src/View/ConnectionDialog.h

FORMS += \
    src/View/EdgeSettingsWidget.ui \
    src/View/MainWindow.ui \
    src/View/ConnectionDialog.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    anticlockwize.png \
    clockwize.png

RESOURCES += \
    resources/Cubiano.qrc
