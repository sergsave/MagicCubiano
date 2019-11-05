#include "PresetEditorWidget.h"
#include "ui_PresetEditorWidget.h"

#include <cassert>

#include "src/Preset/Visitor.h"
#include "GuitarNotationsWidget.h"

#include "Utils.h"

namespace {

enum class Action
{
    RESET,
    SYNC_CLOCKWIZE,
    SYNC_ANTICLOCKWIZE
};

void resetData(auto & preset)
{
    preset.setData({});
}

void syncData(CubeEdge::Rotation rotation, auto & preset)
{
    auto data = preset.data();
    for(auto rot: CubeEdge::allRotations())
    {
        for(auto color: CubeEdge::allColors())
            data[rot][color] = data[rotation][color];
    }
    preset.setData(data);
}

class ActionPresetVisitor : public Preset::Visitor
{
public:
    ActionPresetVisitor(Action action) : m_action(action) {}

    void visit(Preset::GuitarPreset& preset) override
    {
        applyAction(preset);
    }

    void visit(Preset::ElectricGuitarPreset& preset) override
    {
        applyAction(preset);
    }

    void visit(Preset::PianoPreset& ) override
    {
        // TODO Error!
        assert(!"not supported");
    }

private:
    void applyAction(auto & preset)
    {
        switch (m_action)
        {
        case Action::RESET:
            resetData(preset);
            break;
        case Action::SYNC_CLOCKWIZE:
            syncData(CubeEdge::CLOCKWIZE, preset);
            break;
        case Action::SYNC_ANTICLOCKWIZE:
            syncData(CubeEdge::ANTICLOCKWIZE, preset);
            break;
        default:
            break;
        }
    }

    Action m_action;
};

void acceptActionVisitor(Preset::AbstractPreset * preset, Action action)
{
    ActionPresetVisitor visitor(action);
    preset->acceptVisitor(visitor);
}

class CreateEditorPresetVisitor : public Preset::Visitor
{
public:

    CreateEditorPresetVisitor(PresetEditorWidget * parent, const std::function<void()>& presetUpdatedCallback)
        : m_parent(parent)
        , m_callback(presetUpdatedCallback)
    {}

    void visit(Preset::GuitarPreset& preset) override
    {
        using Desc = Instruments::Description<Instruments::GuitarTag>;
        createGuitarWidget(Desc::min(), Desc::max(), preset);
    }

    void visit(Preset::ElectricGuitarPreset& preset) override
    {
        using Desc = Instruments::Description<Instruments::ElectricGuitarTag>;
        createGuitarWidget(Desc::min(), Desc::max(), preset);
    }

    void visit(Preset::PianoPreset& ) override
    {
        // TODO Error!
        assert(!"not supported");
    }

    std::function<void()> presetUpdateFunctor() const { return m_updateFunctor; }
    QWidget * widget() const { return m_widget; }

private:

    void createGuitarWidget(auto min, auto max, auto & preset)
    {
        auto widget = new GuitarNotationsWidget(min, max, m_parent);
        setupWidget(widget, preset);
    }

    void setupWidget(auto * notationsWidget, auto & preset)
    {
        auto parent = m_parent;
        m_updateFunctor = [notationsWidget, &preset, parent] {
            auto edge = parent->activeCubeEdge();
            notationsWidget->setNotations(preset.notations(edge));
        };

        auto callback = m_callback;
        QObject::connect(notationsWidget, &GuitarNotationsWidget::notationsChanged, parent,
            [&preset, callback ,parent](auto notations) {
                preset.setNotations(parent->activeCubeEdge(), notations);
                if(callback)
                    callback();
        });

        m_widget = notationsWidget;
    }

private:
    std::function<void()> m_updateFunctor;
    QWidget * m_widget = nullptr;
    PresetEditorWidget * m_parent = nullptr;
    std::function<void()> m_callback;
};

}

PresetEditorWidget::PresetEditorWidget(QWidget * parent) :
    QWidget(parent),
    m_ui(new Ui::PresetEditorWidget)
{
    m_ui->setupUi(this);

    connect(m_ui->delaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int val) {
        if(!m_preset)
            return;

        m_preset->setMinDurationMs(m_edge, val);
    });

    setLabelPixmap(m_ui->notesLabel, ":/images/musical-note.png");
}

PresetEditorWidget::~PresetEditorWidget() = default;

void PresetEditorWidget::resetData()
{
    acceptActionVisitor(m_preset, Action::RESET);
    updateAll();
}

void PresetEditorWidget::syncDataByClockwize()
{
    acceptActionVisitor(m_preset, Action::SYNC_CLOCKWIZE);
    updateAll();
}

void PresetEditorWidget::syncDataByAnticlockwize()
{
    acceptActionVisitor(m_preset, Action::SYNC_ANTICLOCKWIZE);
    updateAll();
}

void PresetEditorWidget::setAdditionsVisible(bool visible)
{
    m_ui->additionsFrame->setVisible(visible);
}

void PresetEditorWidget::setPreset(Preset::AbstractPreset * preset)
{
    if(!preset)
        return;

    m_preset = preset;

    if(m_editor)
    {
        delete m_editor;
        m_editor = nullptr;
    }

    createEditor(preset);

    auto frame = m_ui->notationsFrame;
    auto layout = frame->layout();

    if(!layout)
        layout = new QHBoxLayout(frame);

    layout->setContentsMargins({});
    layout->addWidget(m_editor);

    updateEditor();
}

void PresetEditorWidget::setActiveCubeEdge(const CubeEdge &edge)
{
    m_edge = edge;
    updateAll();
}

CubeEdge PresetEditorWidget::activeCubeEdge() const
{
    return m_edge;
}

void PresetEditorWidget::createEditor(Preset::AbstractPreset * preset)
{
    updateAdditions();

    CreateEditorPresetVisitor visitor(this, [this] {
        updateAdditions();
    });
    preset->acceptVisitor(visitor);

    m_editor = visitor.widget();
    m_updateFunctor = visitor.presetUpdateFunctor();
}

void PresetEditorWidget::updateAdditions()
{
    if(!m_preset)
        return;

    m_ui->delaySpinBox->setValue(m_preset->minDurationMs(m_edge));
    m_ui->notesValueLabel->setText(tonesToString(m_preset->toHarmony(m_edge).tones));
}

void PresetEditorWidget::updateAll()
{
    updateEditor();
    updateAdditions();
}

// XXX Hack for Android, TODO: remove
bool PresetEditorWidget::event(QEvent *event)
{
    if(m_editor)
        m_editor->setMaximumSize(m_ui->notationsFrame->size());

    return QWidget::event(event);
}

void PresetEditorWidget::updateEditor()
{
    if(m_editor && m_updateFunctor)
        m_updateFunctor();
}


