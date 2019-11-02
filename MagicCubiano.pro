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
    src/Preset/SaveLoader.cpp \
    src/Instruments/Descriptions.cpp \
    src/View/PresetSelectionWidget.cpp \
    src/View/GuitarStringWidget.cpp \
    src/Preset/NotationSerializer.cpp \
    src/View/PresetDialog.cpp \
    src/View/PresetCreatingWidget.cpp \
    src/Preset/Storage.cpp \
    src/View/SaveLoadHelper.cpp \
    src/View/PresetListDialog.cpp \
    src/View/PresetListElementWidget.cpp \
    src/Model/Model.cpp \
    src/Model/PresetModel.cpp \
    src/Model/SettingsModel.cpp \
    src/View/MainPresetEditingWidget.cpp \
    src/View/PresetEditorWidget.cpp \
    src/View/GuitarNotationsWidget.cpp \
    src/Instruments/Common.cpp \
    src/Instruments/Notations.cpp \
    src/Preset/FactoryMethod.cpp \
    src/View/Utils.cpp \
    src/View/NotificationWidget.cpp

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
    src/Instruments/Notations.h \
    src/Instruments/Descriptions.h \
    src/Instruments/Common.h \
    src/Preset/Storage.h \
    src/Preset/SaveLoader.h \
    src/Preset/NotationSerializer.h \
    src/Preset/Serializer.h \
    src/View/PresetSelectionWidget.h \
    src/Preset/Visitor.h \
    src/View/Utils.h \
    src/View/GuitarStringWidget.h \
    src/SoundHelper.h \
    src/View/PresetDialog.h \
    src/View/PresetCreatingWidget.h \
    src/View/SaveLoadHelper.h \
    src/Preset/Presets.h \
    src/View/PresetListDialog.h \
    src/View/PresetListElementWidget.h \
    src/Model/Model.h \
    src/Model/PresetModel.h \
    src/Model/SettingsModel.h \
    src/View/MainPresetEditingWidget.h \
    src/View/PresetEditorWidget.h \
    src/View/GuitarNotationsWidget.h \
    src/Preset/FactoryMethod.h \
    src/View/NotificationWidget.h

FORMS += \
    src/View/ConnectionDialog.ui \
    src/View/PresetSelectionWidget.ui \
    src/View/GuitarStringWidget.ui \
    src/View/MainWindow.ui \
    src/View/PresetCreatingWidget.ui \
    src/View/PresetDialog.ui \
    src/View/PresetListDialog.ui \
    src/View/MainPresetEditingWidget.ui \
    src/View/PresetEditorWidget.ui \
    src/View/NotificationWidget.ui

RESOURCES += \
    resources/Guitar.qrc \
    resources/DistortionGuitar.qrc \
    resources/Other.qrc
