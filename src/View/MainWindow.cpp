#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopWidget>

#include "src/Preset/Storage.h"
#include "PresetDialog.h"
#include "SaveLoadHelper.h"
#include "Utils.h"

MainWindow::MainWindow(Preset::Storage *storage, QWidget *parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_storage(storage)
{
    m_ui->setupUi(this);

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    m_ui->menubar->setNativeMenuBar(false); //fix for menubar notshowing in ubuntu

    updatePresetPage();

    connect(m_ui->createNewButton, &QAbstractButton::clicked, m_ui->newPresetAction, &QAction::trigger);
    connect(m_ui->loadButton, &QAbstractButton::clicked, m_ui->loadPresetAction, &QAction::trigger);

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetEditRequested, this,
            &MainWindow::onEditRequested);

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetRenamed, this,
            [this] (auto oldName, auto newName) {
        m_storage->renamePreset(oldName, newName);
    });

    // Menu actions
    connect(m_ui->newPresetAction, &QAction::triggered, this, &MainWindow::onCreateNew);

    m_ui->savePresetAction->setDisabled(true);
    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::emptyStateChanged, m_ui->savePresetAction,
            &QAction::setDisabled);

    auto saveLoadHelper = new SaveLoadHelper(m_storage, this);

    connect(saveLoadHelper, &SaveLoadHelper::presetLoaded, this, [this](const QString& name) {
        addToSelectionWidget(name);
        updatePresetPage();
    });

    connect(m_ui->loadPresetAction, &QAction::triggered, saveLoadHelper, &SaveLoadHelper::load);

    connect(m_ui->savePresetAction, &QAction::triggered, this, [this, saveLoadHelper] {
        auto name = m_ui->presetSelectionWidget->currentPreset();
        saveLoadHelper->save(name);
    });
}

MainWindow::~MainWindow() = default;

void MainWindow::start()
{
    show();
}

void MainWindow::onEdgeTurned(const CubeEdge& edge)
{

}

void MainWindow::onCreateNew()
{
    PresetDialog dialog;
    auto sourceName = Preset::generateVacantName(*m_storage, "New preset");
    dialog.openCreatePresetPage(sourceName);
    dialog.exec();

    auto preset = dialog.currentPreset();

    if(!preset)
        return;

    auto name = Preset::generateVacantName(*m_storage, dialog.currentPresetName());
    m_storage->addPreset(name, preset);
    addToSelectionWidget(name);

    updatePresetPage();
}

void MainWindow::onEditRequested(const QString &name)
{
    PresetDialog dialog;
    dialog.openEditPresetPage(name, m_storage->findPreset(name));

    dialog.exec();
    updatePresetPage();
}

void MainWindow::updatePresetPage()
{
    if(!m_storage->allPresets().isEmpty())
        m_ui->stackedWidget->setCurrentWidget(m_ui->presetsPage);
    else
        m_ui->stackedWidget->setCurrentWidget(m_ui->noPresetPage);
}

void MainWindow::addToSelectionWidget(const QString &name)
{
    auto inst = instrumentName(m_storage->findPreset(name));
    m_ui->presetSelectionWidget->add(name, inst);
}

