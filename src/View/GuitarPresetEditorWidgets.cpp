#include "GuitarPresetEditorWidgets.h"
#include "GuitarStringWidget.h"

#include <QHBoxLayout>
#include <algorithm>
#include <functional>

using namespace Instruments;

BaseGuitarPresetEditorWidget::BaseGuitarPresetEditorWidget(const GuitarNotation &min,
                                                           const GuitarNotation &max,
                                                           QWidget * parent) :
    BasePresetEditorWidget(parent)
{
    auto layout = new QHBoxLayout(this);

    for(int i = min.string; i <= max.string; ++i)
    {
        auto stringW = new GuitarStringWidget(i, min.fret, max.fret, this);
        layout->addWidget(stringW);

        connect(stringW, &GuitarStringWidget::fretValueChanged, this, [stringW, this](int val) {
            fretChanged(stringW->number(), val);
        });

        connect(stringW, &GuitarStringWidget::muteChaged, this, [stringW, this](bool muted) {
            muteChanged(stringW->number(), muted);
        });

        m_widgets.append(stringW);
    }
}

void BaseGuitarPresetEditorWidget::setState(const BaseGuitarPresetEditorWidget::String2Fret & state)
{
    for(auto w: m_widgets)
        w->reset();

    for(auto string: state.keys())
    {
        m_widgets[string]->setMuted(false);
        m_widgets[string]->setFretValue(state.value(string));
    }
}

using Notations = QList<Instruments::GuitarNotation>;

Notations notationsFrom(const auto& presetData, const CubeEdge& ce)
{
    auto copy = presetData;
    return refNotationsFrom(copy, ce);
}

Notations& refNotationsFrom(auto& presetData, const CubeEdge& ce)
{
    return presetData[ce.rotation][ce.color].notations;
}

QMap<int,int> extractString2Fret(auto * preset, const CubeEdge& ce)
{
    QMap<int, int> ret;

    const auto notations = notationsFrom(preset->data(), ce);

    for(auto n: notations)
        ret.insert(n.string, n.fret);

    return ret;
}

void changePreset(auto * preset, const CubeEdge& ce,
                  const std::function<void(Notations&)>& changeFunc)
{
    auto data = preset->data();
    auto &notations = refNotationsFrom(data, ce);
    changeFunc(notations);
    preset->setData(data);
}

Notations::iterator findNotation(Notations& notations, int string)
{
    return std::find_if(notations.begin(), notations.end(), [string] (auto& notation) {
        return notation.string == string;
    });
}

void onFretChanged(auto * preset, const CubeEdge& ce, int string, int fret)
{
    changePreset(preset, ce, [string, fret] (Notations& notations){

    auto it = findNotation(notations, string);
    if(it == notations.end())
        return;

    it->fret = fret;
    });
}

void onMuteChanged(auto * preset, const CubeEdge& ce, int string, bool mute)
{
    changePreset(preset, ce, [string, mute] (Notations& notations){

    auto it = findNotation(notations, string);

    if(mute)
    {
        if(it != notations.end())
            notations.erase(it);
        return;
    }

    if(it != notations.end())
        return;

    Instruments::GuitarNotation notation;
    notation.string = string;
    notations.append(notation);

    });
}

GuitarPresetEditorWidget::GuitarPresetEditorWidget(Preset::GuitarPreset * preset,
                                                   QWidget *parent) :
    BaseGuitarPresetEditorWidget(Description<GuitarTag>::min(),
                                 Description<GuitarTag>::max(),
                                 parent),
    m_preset(preset)
{
    setState(extractString2Fret(preset, activeCubeEdge()));
}

void GuitarPresetEditorWidget::resetData()
{
    for(auto rot: CubeEdge::allRotations())
    {
        for(auto col: CubeEdge::allColors())
            changePreset(m_preset, {col, rot}, [](Notations& notations){ notations.clear(); });
    }
    setState({});
}

void GuitarPresetEditorWidget::syncDataByClockwize()
{
    for(auto col: CubeEdge::allColors())
    {
        const auto anticlockwizeCe = CubeEdge(col, CubeEdge::ANTICLOCKWIZE);
        const auto clockWizeCe = CubeEdge(col, CubeEdge::CLOCKWIZE);

        changePreset(m_preset, anticlockwizeCe, [this, anticlockwizeCe, clockWizeCe] (Notations& notations) {
            notations = notationsFrom(m_preset->data(), clockWizeCe);
        });
    }

    setState(extractString2Fret(m_preset, activeCubeEdge()));
}

void GuitarPresetEditorWidget::syncDataByAnticlockwize()
{
    for(auto col: CubeEdge::allColors())
    {
        const auto anticlockwizeCe = CubeEdge(col, CubeEdge::ANTICLOCKWIZE);
        const auto clockWizeCe = CubeEdge(col, CubeEdge::CLOCKWIZE);

        changePreset(m_preset, anticlockwizeCe, [this, anticlockwizeCe, clockWizeCe] (Notations& notations) {
            notations = notationsFrom(m_preset->data(), clockWizeCe);
        });
    }

    setState(extractString2Fret(m_preset, activeCubeEdge()));
}

void GuitarPresetEditorWidget::fretChanged(int string, int fret)
{
    onFretChanged(m_preset, activeCubeEdge(), string, fret);
}

void GuitarPresetEditorWidget::muteChanged(int string, bool mute)
{
    onMuteChanged(m_preset, activeCubeEdge(), string, mute);
}

void GuitarPresetEditorWidget::onCubeEdgeChanged(const CubeEdge& edge)
{
    setState(extractString2Fret(m_preset, edge));
}

ElectricGuitarPresetEditorWidget::ElectricGuitarPresetEditorWidget(Preset::ElectricGuitarPreset *preset,
                                                                   QWidget *parent) :
    BaseGuitarPresetEditorWidget(Description<ElectricGuitarTag>::min(),
                                 Description<ElectricGuitarTag>::max(),
                                 parent),
    m_preset(preset)
{
    setState(extractString2Fret(preset, activeCubeEdge()));
}

void ElectricGuitarPresetEditorWidget::resetData()
{
    for(auto rot: CubeEdge::allRotations())
    {
        for(auto col: CubeEdge::allColors())
            changePreset(m_preset, {col, rot}, [](Notations& notations){ notations.clear(); });
    }
    setState({});
}

void ElectricGuitarPresetEditorWidget::syncDataByClockwize()
{
    for(auto col: CubeEdge::allColors())
    {
        const auto anticlockwizeCe = CubeEdge(col, CubeEdge::ANTICLOCKWIZE);
        const auto clockWizeCe = CubeEdge(col, CubeEdge::CLOCKWIZE);

        changePreset(m_preset, anticlockwizeCe, [this, anticlockwizeCe, clockWizeCe] (Notations& notations) {
            notations = notationsFrom(m_preset->data(), clockWizeCe);
        });
    }

    setState(extractString2Fret(m_preset, activeCubeEdge()));
}

void ElectricGuitarPresetEditorWidget::syncDataByAnticlockwize()
{
    for(auto col: CubeEdge::allColors())
    {
        const auto anticlockwizeCe = CubeEdge(col, CubeEdge::ANTICLOCKWIZE);
        const auto clockWizeCe = CubeEdge(col, CubeEdge::CLOCKWIZE);

        changePreset(m_preset, anticlockwizeCe, [this, anticlockwizeCe, clockWizeCe] (Notations& notations) {
            notations = notationsFrom(m_preset->data(), clockWizeCe);
        });
    }

    setState(extractString2Fret(m_preset, activeCubeEdge()));
}

void ElectricGuitarPresetEditorWidget::fretChanged(int string, int fret)
{
    onFretChanged(m_preset, activeCubeEdge(), string, fret);
}

void ElectricGuitarPresetEditorWidget::muteChanged(int string, bool mute)
{
    onMuteChanged(m_preset, activeCubeEdge(), string, mute);
}

void ElectricGuitarPresetEditorWidget::onCubeEdgeChanged(const CubeEdge& edge)
{
    setState(extractString2Fret(m_preset, edge));
}

