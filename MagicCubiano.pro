#-------------------------------------------------
#
# Project created by QtCreator 2018-11-17T14:53:59
#
#-------------------------------------------------

CONFIG += c++14

QT       += core gui bluetooth multimedia

android{
    QT       += androidextras
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

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
    src/View/MainWindow.cpp \
    src/Protocol/GiikerProtocol.cpp \
    src/main.cpp \
    src/View/ConnectionDialog.cpp \
    src/View/EdgeWidget.cpp \
    src/View/GuitarEdgeSettingsDialog.cpp \
    src/Music.cpp \
    src/Sound/ToneGenerator.cpp \
    src/Sound/SampleGenerator.cpp \
    src/Sound/GuitarGenerators.cpp \
    src/View/InstrumentSelectionWidget.cpp \
    src/View/EdgeSettingsFactory.cpp

HEADERS += \
    src/View/MainWindow.h \
    src/Protocol/GiikerProtocol.h \
    src/View/ConnectionDialog.h \
    src/CubeEdge.h \
    src/View/EdgeWidget.h \
    src/View/EdgeSettingsFactory.h \
    src/View/GuitarEdgeSettingsDialog.h \
    src/View/EdgeSettingsDialog.h \
    src/Music.h \
    src/Sound/ToneGenerator.h \
    src/Sound/SoundGenerator.h \
    src/Sound/SoundGeneratorFactory.h \
    src/Sound/SampleGenerator.h \
    src/Configuration.h \
    src/Sound/GuitarGenerators.h \
    src/View/InstrumentSelectionWidget.h

FORMS += \
    src/View/MainWindow.ui \
    src/View/ConnectionDialog.ui \
    src/View/EdgeWidget.ui \
    src/View/GuitarEdgeSettingsDialog.ui \
    src/View/InstrumentSelectionWidget.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    anticlockwize.png \
    clockwize.png \
    android-sources/AndroidManifest.xml

RESOURCES += \
    resources/Cubiano.qrc
