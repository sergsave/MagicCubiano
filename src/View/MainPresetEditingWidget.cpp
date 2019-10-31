#include "MainPresetEditingWidget.h"
#include "ui_MainPresetEditingWidget.h"

#include <cassert>
#include <QMenu>

#include "Utils.h"

MainPresetEditingWidget::MainPresetEditingWidget(QWidget * parent):
    QWidget(parent),
    m_ui(new Ui::MainPresetEditingWidget)
{
    m_ui->setupUi(this);

    auto group = new QButtonGroup(this);
    for (auto b: selectors().keys())
        group->addButton(b);

    connect(group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this] (auto button) {
        m_ui->editorWidget->setActiveCubeEdge(this->selectors()[button]);
    });

    connect(m_ui->applyButton, &QAbstractButton::clicked, this, &MainPresetEditingWidget::finished);

    auto menu = createMenu();
    connect(m_ui->menuButton, &QAbstractButton::clicked, menu, [menu, this] {
        const auto buttonSize = m_ui->menuButton->size();
        const auto origin = m_ui->menuButton->mapToGlobal({ buttonSize.width(), buttonSize.height()});
        const auto menuWidth = menu->sizeHint().width();
        const auto pos = origin - QPoint(menuWidth, 0);

        menu->popup(pos);
    });

    m_ui->editorWidget->setAdditionsVisible(false);
}

void MainPresetEditingWidget::setPreset(const QString &name, Preset::AbstractPreset *preset)
{
    for(auto action: m_actions.values())
        action->setEnabled(preset != nullptr);

    assert(preset);

    m_ui->titleLabel->setText(QString("%1 (%2)").arg(name, instrumentName(preset)));
    m_ui->editorWidget->setPreset(preset);

    // Always start with edge of first button
    m_ui->editorWidget->setActiveCubeEdge(selectors()[m_ui->clockYellowButton]);
}

QMap<QAbstractButton *, CubeEdge> MainPresetEditingWidget::selectors() const
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

QMenu * MainPresetEditingWidget::createMenu()
{
    const QString g_resetAction = "Reset";
    const QString g_clockwizeSyncAction = "Sync by clockwize";
    const QString g_anticlockwizeSyncAction = "Sync by anticlockwize";
    const QString g_additionsAction = "Show additions";

    auto menu = new QMenu(this);

    auto initAction = [this, menu](const QString& name) {
        return m_actions[name] = menu->addAction(name);
    };

    auto resetAction = initAction(g_resetAction);
    auto clockwizeAction = initAction(g_clockwizeSyncAction);
    auto anticlockwizeAction = initAction(g_anticlockwizeSyncAction);
    auto additionsAction = initAction(g_additionsAction);

    additionsAction->setCheckable(true);

    connect(resetAction, &QAction::triggered, m_ui->editorWidget,
            &PresetEditorWidget::resetData);
    connect(clockwizeAction, &QAction::triggered, m_ui->editorWidget,
            &PresetEditorWidget::syncDataByClockwize);
    connect(anticlockwizeAction, &QAction::triggered, m_ui->editorWidget,
            &PresetEditorWidget::syncDataByAnticlockwize);
    connect(additionsAction, &QAction::toggled, m_ui->editorWidget,
            &PresetEditorWidget::setAdditionsVisible);

    return menu;
}

MainPresetEditingWidget::~MainPresetEditingWidget() = default;
