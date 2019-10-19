#include "GuitarPresetEditorWidgets.h"
#include "GuitarStringWidget.h"

#include <QHBoxLayout>
#include <algorithm>

using namespace Instruments;

BaseGuitarPresetEditorWidget::BaseGuitarPresetEditorWidget(const GuitarNotation &min,
                                                           const GuitarNotation &max,
                                                           const QMap<int, int> &string2fret,
                                                           QWidget * parent) :
    BasePresetEditorWidget(parent)

{
    auto layout = new QHBoxLayout(this);

    for(int i = min.string; i <= max.string; ++i)
    {
        auto stringW = new GuitarStringWidget(i, min.fret, max.fret, this);
        layout->addWidget(stringW);

        auto initFret = string2fret.value(i, -1);
        if(initFret != -1)
        {
            // Before connections, just init
            stringW->setMuted(false);
            stringW->setFretValue(initFret);
        }

        connect(stringW, &GuitarStringWidget::fretValueChanged, this, [stringW, this](int val) {
            fretChanged(stringW->number(), val);
        });

        connect(stringW, &GuitarStringWidget::muteChaged, this, [stringW, this](bool muted) {
            muteChanged(stringW->number(), muted);
        });
    }
}

QMap<int,int> extractString2Fret(auto * preset, const CubeEdge& ce)
{
    QMap<int, int> ret;
    auto units = preset->data()[ce.rotation][ce.color];
    for(auto u: units)
        ret.insert(u.notation.string, u.notation.fret);

    return ret;
}

void onFretChanged(auto * preset, const CubeEdge& ce, int string, int fret)
{
    auto data = preset->data();
    auto& units = data[ce.rotation][ce.color];
    auto it = std::find_if(units.begin(), units.end(), [string] (const auto& unit) {
        return unit.notation.string == string;
    });

    if(it == units.end())
        return;

    it->notation.fret = fret;
    preset->setData(data);
}

void onMuteChanged(auto * preset, const CubeEdge& ce, int string, bool mute)
{
    auto data = preset->data();
    auto& units = data[ce.rotation][ce.color];

    if(!mute)
    {
        auto it = std::find_if(units.begin(), units.end(), [string] (const auto& unit) {
            return unit.notation.string == string;
        });

        if(it == units.end())
            return;
    }
    it->notation.fret = fret;

    preset->setData(data);
}

GuitarPresetEditorWidget::GuitarPresetEditorWidget(Preset::GuitarPreset * preset,
                                                   QWidget *parent) :
    BaseGuitarPresetEditorWidget(Description<GuitarTag>::min(),
                                 Description<GuitarTag>::max(),
                                 extractString2Fret(preset, activeCubeEdge()),
                                 parent),
    m_preset(preset)
{

}

void GuitarPresetEditorWidget::fretChanged(int string, int fret)
{
    onFretChanged(m_preset, activeCubeEdge(), string, fret);
}

void GuitarPresetEditorWidget::muteChanged(int string, bool mute)
{

}

ElectricGuitarPresetEditorWidget::ElectricGuitarPresetEditorWidget(Preset::ElectricGuitarPreset *preset,
                                                                   QWidget *parent) :
    BaseGuitarPresetEditorWidget(Description<ElectricGuitarTag>::min(),
                                 Description<ElectricGuitarTag>::max(),
                                 extractString2Fret(preset, activeCubeEdge()),
                                 parent),
    m_preset(preset)
{

}

void ElectricGuitarPresetEditorWidget::fretChanged(int string, int fret)
{
    onFretChanged(m_preset, activeCubeEdge(), string, fret);
}

void ElectricGuitarPresetEditorWidget::muteChanged(int string, bool mute)
{

}
