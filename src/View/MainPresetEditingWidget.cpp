#include "MainPresetEditingWidget.h"
#include "ui_MainPresetEditingWidget.h"

#include <cassert>
#include <QMenu>
#include <QTimer>

#include "Utils.h"

namespace {

QString backgroundColorStyleSheet(const QColor& col)
{
    return QString("background-color: %1").arg(col.name());
}

}

MainPresetEditingWidget::MainPresetEditingWidget(QWidget * parent):
    QWidget(parent),
    m_ui(new Ui::MainPresetEditingWidget)
{
    m_ui->setupUi(this);

    auto group = new QButtonGroup(this);
    for (auto b: selectors().keys())
    {
        b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        group->addButton(b);
    }

    m_ui->clockYellowButton->setChecked(true);

    connect(group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this] (auto button) {
        m_ui->editorWidget->setActiveCubeEdge(this->selectors()[button]);
    });

    connect(m_ui->applyButton, &QAbstractButton::clicked, this, &MainPresetEditingWidget::finished);

    auto menu = createMenu();
    bindMenu(menu, m_ui->menuButton);

    m_ui->editorWidget->setAdditionsVisible(false);
}

void MainPresetEditingWidget::onEdgeTurned(const CubeEdge& ce)
{
    // Selectors size is fixed
    auto button = selectors().key(ce);
    auto prevStylesheet = button->styleSheet();

    button->setStyleSheet({});

    const int blinkTime = 200;
    QTimer::singleShot(blinkTime, button, [prevStylesheet, button] {
        button->setStyleSheet(prevStylesheet);
    });
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
    auto menu = new QMenu(this);

    auto initAction = [this, menu](const QString& name) {
        return m_actions[name] = menu->addAction(name);
    };

    auto resetAction = initAction("Reset");
    auto clockwizeAction = initAction("Sync by clockwize");
    auto anticlockwizeAction = initAction("Sync by anticlockwize");
    auto additionsAction = initAction("Show additions");

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

// For correct icons size
void MainPresetEditingWidget::showEvent(QShowEvent * ev)
{
    for(auto b: selectors().keys())
    {
        auto edge = selectors().value(b);
        auto appearance = appearanceFor(edge);

        auto pm = QPixmap(appearance.iconPath);
        b->setIconSize(b->size() * 3 / 4);
        b->setIcon(pm);
        b->setStyleSheet(backgroundColorStyleSheet(appearance.color));
    }
    QWidget::showEvent(ev);
}

MainPresetEditingWidget::~MainPresetEditingWidget() = default;
