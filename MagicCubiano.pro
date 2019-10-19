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
    src/Music.cpp \
    src/Sound/SampleGenerator.cpp \
    src/Sound/GuitarGenerators.cpp \
    src/View/SettingsDialog.cpp \
    src/Preset/SaveLoader.cpp \
    src/Instruments/Descriptions.cpp \
    src/View/PresetSelectionWidget.cpp \
    src/View/GuitarPresetEditorWidgets.cpp \
    src/View/GuitarStringWidget.cpp \
    src/Preset/NotationSerializer.cpp \
    src/View/PresetDialog.cpp \
    src/View/PresetEditingWidget.cpp \
    src/View/PresetCreatingWidget.cpp \
    src/Preset/Storage.cpp

HEADERS += \
    src/View/MainWindow.h \
    src/Protocol/GiikerProtocol.h \
    src/View/ConnectionDialog.h \
    src/CubeEdge.h \
    src/Music.h \
    src/Sound/SoundGenerator.h \
    src/Sound/SoundGeneratorFactory.h \
    src/Sound/SampleGenerator.h \
    src/Configuration.h \
    src/Sound/GuitarGenerators.h \
    src/View/SettingsDialog.h \
    src/Instruments/Notations.h \
    src/Instruments/Descriptions.h \
    src/Instruments/Common.h \
    src/Preset/Storage.h \
    src/Preset/SaveLoader.h \
    src/Preset/NotationSerializer.h \
    src/Preset/Serializer.h \
    src/View/PresetSelectionWidget.h \
    src/Preset/Model.h \
    src/Preset/Visitor.h \
    src/View/BasePresetEditorWidget.h \
    src/View/GuitarPresetEditorWidgets.h \
    src/View/Utils.h \
    src/View/GuitarStringWidget.h \
    src/SoundHelper.h \
    src/View/PresetDialog.h \
    src/View/PresetEditingWidget.h \
    src/View/PresetCreatingWidget.h

FORMS += \
    src/View/ConnectionDialog.ui \
    src/View/SettingsDialog.ui \
    src/View/PresetSelectionWidget.ui \
    src/View/GuitarStringWidget.ui \
    src/View/MainWindow.ui \
    src/View/PresetEditingWidget.ui \
    src/View/PresetCreatingWidget.ui \
    src/View/PresetDialog.ui

RESOURCES += \
    resources/Guitar.qrc \
    resources/DistortionGuitar.qrc \
    resources/Other.qrc
