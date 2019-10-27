#pragma once

#include <QScopedPointer>

#include "BasePresetEditorWidget.h"
#include "src/Preset/Visitor.h"

class GuitarStringWidget;
class IGuitarPresetEditor;

class BaseGuitarPresetEditorWidget : public BasePresetEditorWidget
{
    Q_OBJECT
public:
    explicit BaseGuitarPresetEditorWidget(IGuitarPresetEditor * impl, QWidget * parent);
    ~BaseGuitarPresetEditorWidget();

    void resetData() override;
    void syncDataByClockwize() override;
    void syncDataByAnticlockwize() override;

    void onCubeEdgeChanged(const CubeEdge&) override;

private:
    void syncUiWithImpl();

    void fretChanged(int string, int fret);
    void muteChanged(int string, bool mute);

private:
    QScopedPointer<IGuitarPresetEditor> m_impl;
    QList<GuitarStringWidget*> m_widgets;
};

class GuitarPresetEditorWidget : public BaseGuitarPresetEditorWidget
{
    Q_OBJECT

public:
    explicit GuitarPresetEditorWidget(Preset::GuitarPreset * preset, QWidget * parent = nullptr);
};

class ElectricGuitarPresetEditorWidget : public BaseGuitarPresetEditorWidget
{
    Q_OBJECT

public:
    explicit ElectricGuitarPresetEditorWidget(Preset::ElectricGuitarPreset * preset, QWidget * parent = nullptr);
};

