#pragma once

#include "BasePresetEditorWidget.h"
#include "src/Preset/Presets.h"

class GuitarStringWidget;

class BaseGuitarPresetEditorWidget : public BasePresetEditorWidget
{
    Q_OBJECT
public:
    explicit BaseGuitarPresetEditorWidget(const Instruments::GuitarNotation& min,
                                          const Instruments::GuitarNotation& max,
                                          QWidget * parent);

protected:
    using String2Fret = QMap<int, int>;
    void setState(const String2Fret&);

    virtual void fretChanged(int string, int fret) = 0;
    virtual void muteChanged(int string, bool mute) = 0;

private:
    QList<GuitarStringWidget*> m_widgets;
};

class GuitarPresetEditorWidget : public BaseGuitarPresetEditorWidget
{
    Q_OBJECT
public:
    explicit GuitarPresetEditorWidget(Preset::GuitarPreset * preset, QWidget * parent = nullptr);

    void resetData() override;
    void syncDataByClockwize() override;
    void syncDataByAnticlockwize() override;

protected:
    void fretChanged(int string, int fret) override;
    void muteChanged(int string, bool mute) override;
    void onCubeEdgeChanged(const CubeEdge&) override;

private:
    Preset::GuitarPreset * m_preset = nullptr;
};

class ElectricGuitarPresetEditorWidget : public BaseGuitarPresetEditorWidget
{
    Q_OBJECT
public:
    explicit ElectricGuitarPresetEditorWidget(Preset::ElectricGuitarPreset * preset, QWidget * parent = nullptr);

    void resetData() override;
    void syncDataByClockwize() override;
    void syncDataByAnticlockwize() override;

protected:
    void fretChanged(int string, int fret) override;
    void muteChanged(int string, bool mute) override;
    void onCubeEdgeChanged(const CubeEdge&) override;

private:
    Preset::ElectricGuitarPreset * m_preset = nullptr;
};
