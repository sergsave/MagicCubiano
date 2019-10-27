#include "PresetEditingWidget.h"
#include "ui_PresetEditingWidget.h"

#include <cassert>

#include <QMenu>

#include "GuitarPresetEditorWidgets.h"
#include "src/Preset/Visitor.h"
#include "Utils.h"

namespace {

const QString g_resetAction = "Reset";
const QString g_clockwizeSyncAction = "Sync by clockwize";
const QString g_anticlockwizeSyncAction = "Sync by anticlockwize";
const QString g_additionsAction = "Show additions";

class EditingPresetVisitor : public Preset::Visitor
{
public:

    EditingPresetVisitor(QWidget * parent) : m_parent(parent) {}

    void visit(Preset::GuitarPreset& preset) override
    {
        m_widget = new GuitarPresetEditorWidget(&preset, m_parent);
    }

    void visit(Preset::ElectricGuitarPreset& preset) override
    {
        m_widget = new ElectricGuitarPresetEditorWidget(&preset, m_parent);
    }

    void visit(Preset::PianoPreset& ) override
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
    m_ui->additionsFrame->setVisible(false);

    auto group = new QButtonGroup(this);
    for (auto b: selectors().keys())
        group->addButton(b);

    connect(group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this] (auto button) {
        m_editor->setActiveCubeEdge(this->selectors()[button]);
    });

    connect(m_ui->applyButton, &QAbstractButton::clicked, this, &PresetEditingWidget::finished);

    auto menu = createMenu();
    connect(m_ui->menuButton, &QAbstractButton::clicked, menu, [menu, this] {
        const auto buttonSize = m_ui->menuButton->size();
        const auto origin = m_ui->menuButton->mapToGlobal({ buttonSize.width(), buttonSize.height()});
        const auto menuWidth = menu->sizeHint().width();
        const auto pos = origin - QPoint(menuWidth, 0);

        menu->popup(pos);
    });
}

void PresetEditingWidget::setPreset(const QString &name, Preset::AbstractPreset *preset)
{
    assert(preset);

    m_ui->titleLabel->setText(QString("%1 (%2)").arg(name, instrumentName(preset)));

    EditingPresetVisitor editVisitor(this);
    preset->acceptVisitor(editVisitor);

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

    layout->setContentsMargins({});
    layout->addWidget(editor);

    // Always start with edge of first button
    editor->setActiveCubeEdge(selectors()[m_ui->clockYellowButton]);

    m_editor = editor;
    bindActions(m_editor);
    updateActionsState();
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

QMenu * PresetEditingWidget::createMenu()
{
    auto menu = new QMenu(this);

    auto initAction = [this, menu](const QString& name) {
        return m_actions[name] = menu->addAction(name);
    };

    initAction(g_resetAction);
    initAction(g_clockwizeSyncAction);
    initAction(g_anticlockwizeSyncAction);

    auto action = initAction(g_additionsAction);
    action->setCheckable(true);
    connect(action, &QAction::toggled, m_ui->additionsFrame, &QFrame::setVisible);
    return menu;
}

void PresetEditingWidget::bindActions(BasePresetEditorWidget * editor)
{
    connect(m_actions[g_resetAction], &QAction::triggered, editor, &BasePresetEditorWidget::resetData);
    connect(m_actions[g_clockwizeSyncAction], &QAction::triggered, editor,
            &BasePresetEditorWidget::syncDataByClockwize);
    connect(m_actions[g_anticlockwizeSyncAction], &QAction::triggered, editor,
            &BasePresetEditorWidget::syncDataByAnticlockwize);
}

void PresetEditingWidget::updateActionsState()
{
    for(auto action: m_actions.values())
        action->setEnabled(m_editor != nullptr);
}

PresetEditingWidget::~PresetEditingWidget() = default;
