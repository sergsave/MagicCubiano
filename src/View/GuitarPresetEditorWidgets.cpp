#include "GuitarPresetEditorWidgets.h"
#include "GuitarStringWidget.h"

#include <QHBoxLayout>

#include "GuitarPresetEditorImpl.h"

BaseGuitarPresetEditorWidget::BaseGuitarPresetEditorWidget(IGuitarPresetEditor *impl, QWidget * parent) :
    BasePresetEditorWidget(parent),
    m_impl(impl)
{
    auto layout = new QHBoxLayout(this);

    auto min = m_impl->min();
    auto max = m_impl->max();

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

BaseGuitarPresetEditorWidget::~BaseGuitarPresetEditorWidget() = default;

void BaseGuitarPresetEditorWidget::syncUiWithImpl()
{
    auto state = m_impl->stateFor(activeCubeEdge());

    for(auto w: m_widgets)
        w->reset();

    for(auto string: state.keys())
    {
        m_widgets[string]->setMuted(false);
        m_widgets[string]->setFretValue(state.value(string));
    }
}

void BaseGuitarPresetEditorWidget::resetData()
{
    m_impl->resetData();
    syncUiWithImpl();
}

void BaseGuitarPresetEditorWidget::syncDataByClockwize()
{
    m_impl->syncDataByClockwize();
    syncUiWithImpl();
}

void BaseGuitarPresetEditorWidget::syncDataByAnticlockwize()
{
    m_impl->syncDataByAnticlockwize();
    syncUiWithImpl();
}

void BaseGuitarPresetEditorWidget::onCubeEdgeChanged(const CubeEdge &)
{
    syncUiWithImpl();
}


void BaseGuitarPresetEditorWidget::fretChanged(int string, int fret)
{
    m_impl->onFretChanged(activeCubeEdge(), string, fret);
}

void BaseGuitarPresetEditorWidget::muteChanged(int string, bool mute)
{
    m_impl->onMuteChanged(activeCubeEdge(), string, mute);
}

using namespace Instruments;

GuitarPresetEditorWidget::GuitarPresetEditorWidget(
    Preset::GuitarPreset *preset,
    QWidget *parent)
    : BaseGuitarPresetEditorWidget(new GuitarPresetEditorImpl<GuitarTag>(preset), parent)
{}

ElectricGuitarPresetEditorWidget::ElectricGuitarPresetEditorWidget(
    Preset::ElectricGuitarPreset *preset,
    QWidget *parent)
    : BaseGuitarPresetEditorWidget(new GuitarPresetEditorImpl<ElectricGuitarTag>(preset), parent)
{}


