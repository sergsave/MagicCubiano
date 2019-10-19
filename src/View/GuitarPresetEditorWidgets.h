#pragma once

#include "BasePresetEditorWidget.h"
#include "src/Preset/Model.h"

class BaseGuitarPresetEditorWidget : public BasePresetEditorWidget
{
    Q_OBJECT
public:
    explicit BaseGuitarPresetEditorWidget(const Instruments::GuitarNotation& min,
                                          const Instruments::GuitarNotation& max,
                                          const QMap<int, int>& string2fret,
                                          QWidget * parent);

protected:
    virtual void fretChanged(int string, int fret) {}
    virtual void muteChanged(int string, bool mute) {}
};

class GuitarPresetEditorWidget : public BaseGuitarPresetEditorWidget
{
    Q_OBJECT

public:
    explicit GuitarPresetEditorWidget(Preset::GuitarPreset * preset, QWidget * parent = nullptr);

protected:
    void fretChanged(int string, int fret) override;
    void muteChanged(int string, bool mute) override;

private:
    Preset::GuitarPreset * m_preset = nullptr;
};

class ElectricGuitarPresetEditorWidget : public BaseGuitarPresetEditorWidget
{
    Q_OBJECT

public:
    explicit ElectricGuitarPresetEditorWidget(Preset::ElectricGuitarPreset * preset, QWidget * parent = nullptr);

protected:
    void fretChanged(int string, int fret) override;
    void muteChanged(int string, bool mute) override;

private:
    Preset::ElectricGuitarPreset * m_preset = nullptr;
};
