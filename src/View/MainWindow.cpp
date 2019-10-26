#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopWidget>

#include "src/Preset/Model.h"
#include "PresetDialog.h"
#include "SaveLoadHelper.h"
#include "PresetListDialog.h"
#include "Utils.h"

#include <QDebug>

MainWindow::MainWindow(Preset::Model * model, QWidget *parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_model(model)
{
    m_ui->setupUi(this);

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    m_ui->menubar->setNativeMenuBar(false); //fix for menubar notshowing in ubuntu

    updatePresetPage();
    updateActionsState();

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetOpenRequested, this,
        &MainWindow::onOpenRequested);

    connect(m_ui->createNewButton, &QAbstractButton::clicked, m_ui->newPresetAction, &QAction::trigger);
    connect(m_ui->loadButton, &QAbstractButton::clicked, m_ui->loadPresetAction, &QAction::trigger);

    auto saveLoadHelper = new SaveLoadHelper(this);
    connect(m_ui->newPresetAction, &QAction::triggered, this, &MainWindow::onCreateNew);
    connect(m_ui->loadPresetAction, &QAction::triggered, saveLoadHelper, &SaveLoadHelper::startLoading);
    connect(m_ui->savePresetAction, &QAction::triggered, this, [saveLoadHelper, this] {
        saveLoadHelper->startSaving(m_model->activePreset());
    });
    connect(m_ui->presetsListAction, &QAction::triggered, this, &MainWindow::showPresetListDialog);

    // TODO: Handle save-load errors
    connect(saveLoadHelper, &SaveLoadHelper::loadRequested, m_model, &Preset::Model::load);
    connect(saveLoadHelper, &SaveLoadHelper::saveRequested, m_model, &Preset::Model::save);

    connect(m_model, &Preset::Model::changed, this, [this] {
        updatePresetPage();
        updateActionsState();

        const QSignalBlocker blocker(m_ui->presetSelectionWidget);
        Q_UNUSED(blocker);
        m_ui->presetSelectionWidget->setPresets(m_model->allPresets());
        m_ui->presetSelectionWidget->setSelectedPreset(m_model->activePreset());

        for(auto preset: m_model->allPresets())
        {
            auto instrument = instrumentName(m_model->findPreset(preset));
            m_ui->presetSelectionWidget->setPresetAdditionalInfo(preset, instrument);
        }
    });

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetSelected, m_model,
        &Preset::Model::setActivePreset);
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
    auto sourceName = m_model->findVacantName("New preset");
    dialog.openCreatePresetPage(sourceName);
    dialog.exec();

    auto preset = dialog.currentPreset();

    if(!preset)
        return;

    auto name = m_model->findVacantName(dialog.currentPresetName());
    m_model->addPreset(name, preset);

    updatePresetPage();
}

void MainWindow::onOpenRequested(const QString &name)
{
    PresetDialog dialog;
    dialog.openEditPresetPage(name, m_model->findPreset(name));

    dialog.exec();
    updatePresetPage();
}

void MainWindow::showPresetListDialog()
{
    PresetListDialog dialog;
    dialog.setPresetList(m_model->allPresets());
    auto res = dialog.exec();

    if(res == QDialog::Rejected)
        return;

    auto renamed = dialog.renamedPresets();
    for(auto oldName: renamed.keys())
    {
        auto newName = renamed.value(oldName);
        m_model->renamePreset(oldName, newName);
    }

    for(auto name: dialog.removedPresets())
        m_model->removePreset(name);
}

void MainWindow::updatePresetPage()
{
    if(!m_model->allPresets().isEmpty())
        m_ui->stackedWidget->setCurrentWidget(m_ui->presetsPage);
    else
        m_ui->stackedWidget->setCurrentWidget(m_ui->noPresetPage);
}

void MainWindow::updateActionsState()
{
    auto isEmpty = m_model->allPresets().isEmpty();

    m_ui->savePresetAction->setDisabled(isEmpty);
    m_ui->presetsListAction->setDisabled(isEmpty);
}


