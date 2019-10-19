#include "PresetEditingWidget.h"
#include "ui_PresetEditingWidget.h"

#include <cassert>

#include "GuitarPresetEditorWidgets.h"
#include "src/Preset/Visitor.h"
#include "Utils.h"

namespace {

class EditingPresetVisitor : public Preset::Visitor
{
public:

    EditingPresetVisitor(QWidget * parent) : m_parent(parent) {}

    void visit(Preset::GuitarPreset * preset) override
    {
        m_widget = new GuitarPresetEditorWidget(preset, m_parent);
    }

    void visit(Preset::ElectricGuitarPreset * preset) override
    {
        m_widget = new ElectricGuitarPresetEditorWidget(preset, m_parent);
    }

    void visit(Preset::PianoPreset *) override
    {
        // TODO Error!
        assert(!"not supported");
    }

    BasePresetEditorWidget * editor() const { return m_widget; }

private:
    BasePresetEditorWidget * m_widget = nullptr;
    QWidget * m_parent;
};

}

PresetEditingWidget::PresetEditingWidget(QWidget * parent):
    QWidget(parent),
    m_ui(new Ui::PresetEditingWidget)
{
    m_ui->setupUi(this);

    auto group = new QButtonGroup(this);
    for (auto b: selectors().keys())
        group->addButton(b);

    connect(group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this] (auto button) {
        m_editor->setActiveCubeEdge(this->selectors()[button]);
    });

    connect(m_ui->applyButton, &QAbstractButton::clicked, this, &PresetEditingWidget::finished);
}

void PresetEditingWidget::setPreset(const QString &name, Preset::AbstractPreset *preset)
{
    assert(preset);

    m_ui->nameLabel->setText(name);
    m_ui->instrumentLabel->setText(instrumentName(preset));

    EditingPresetVisitor editVisitor(this);
    preset->acceptVisitor(&editVisitor);

    setEditorWidget(editVisitor.editor());
}

void PresetEditingWidget::setEditorWidget(BasePresetEditorWidget * editor)
{
    if(m_editor)
        delete m_editor;

    auto frame = m_ui->editorFrame;
    auto layout = frame->layout();

    if(!layout)
        layout = new QHBoxLayout(frame);

    layout->addWidget(editor);
    m_editor = editor;
}

QMap<QAbstractButton *, CubeEdge> PresetEditingWidget::selectors() const
{
    const QMap<QAbstractButton *, CubeEdge> map
    {
        { m_ui->clockYellowButton,  { CubeEdge::YELLOW, CubeEdge::CLOCKWIZE     } },
        { m_ui->aclockYellowButton, { CubeEdge::YELLOW, CubeEdge::ANTICLOCKWIZE } },
        { m_ui->clockRedButton,     { CubeEdge::RED,    CubeEdge::CLOCKWIZE     } },
        { m_ui->aclockRedButton,    { CubeEdge::RED,    CubeEdge::ANTICLOCKWIZE } },
        { m_ui->clockOrangeButton,  { CubeEdge::ORANGE, CubeEdge::CLOCKWIZE     } },
        { m_ui->aclockOrangeButton, { CubeEdge::ORANGE, CubeEdge::ANTICLOCKWIZE } },
        { m_ui->clockGreenButton,   { CubeEdge::GREEN,  CubeEdge::CLOCKWIZE     } },
        { m_ui->aclockGreenButton,  { CubeEdge::GREEN,  CubeEdge::ANTICLOCKWIZE } },
        { m_ui->clockWhiteButton,   { CubeEdge::WHITE,  CubeEdge::CLOCKWIZE     } },
        { m_ui->aclockWhiteButton,  { CubeEdge::WHITE,  CubeEdge::ANTICLOCKWIZE } },
        { m_ui->clockBlueButton,    { CubeEdge::BLUE,   CubeEdge::CLOCKWIZE     } },
        { m_ui->aclockBlueButton,   { CubeEdge::BLUE,   CubeEdge::ANTICLOCKWIZE } }
    };

    return map;
}

PresetEditingWidget::~PresetEditingWidget() = default;
