#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopWidget>

#include "src/Preset/Presets.h"
#include "PresetDialog.h"
#include "SaveLoadHelper.h"
#include "PresetListDialog.h"
#include "Utils.h"

#include <QTimer>

MainWindow::MainWindow(Model * model, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_presetModel(model->presets())
    , m_settings(model->settings())
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
    connect(m_ui->loadAllButton, &QAbstractButton::clicked, m_ui->loadAllPresetsAction, &QAction::trigger);

    auto saveLoadHelper = new SaveLoadHelper(this);
    connect(m_ui->newPresetAction, &QAction::triggered, this, &MainWindow::onCreateNew);
    connect(m_ui->loadPresetAction, &QAction::triggered, saveLoadHelper, &SaveLoadHelper::choosePathForLoading);
    connect(m_ui->loadAllPresetsAction, &QAction::triggered, saveLoadHelper,
        &SaveLoadHelper::choosePathForLoadingAll);
    connect(m_ui->savePresetAction, &QAction::triggered, this, [saveLoadHelper, this] {
        saveLoadHelper->choosePathForSaving(m_presetModel->activePreset());
    });
    connect(m_ui->presetsListAction, &QAction::triggered, this, &MainWindow::showPresetListDialog);

    // TODO: Handle save-load errors
    connect(saveLoadHelper, &SaveLoadHelper::loadRequested, m_presetModel, &PresetModel::loadPreset);
    connect(saveLoadHelper, &SaveLoadHelper::loadAllRequested, m_presetModel, &PresetModel::loadAllPresets);
    connect(saveLoadHelper, &SaveLoadHelper::saveRequested, m_presetModel, &PresetModel::savePresets);

    connect(m_presetModel, &PresetModel::changed, this, [this] {
        updatePresetPage();
        updateActionsState();

        const QSignalBlocker blocker(m_ui->presetSelectionWidget);
        Q_UNUSED(blocker);
        m_ui->presetSelectionWidget->setPresets(m_presetModel->allPresets());
        m_ui->presetSelectionWidget->setSelectedPreset(m_presetModel->activePreset());

        for(auto preset: m_presetModel->allPresets())
        {
            auto instrument = instrumentName(m_presetModel->findPreset(preset));
            m_ui->presetSelectionWidget->setPresetAdditionalInfo(preset, instrument);
        }
    });

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetSelected, m_presetModel,
        &PresetModel::setActivePreset);

    auto syncSettingsUi = [this] {
        int vol = m_settings->volume();
        m_ui->volumeSlider->setValue(vol);
        m_ui->volumeLabel->setText(QString("Vol. \n%1").arg(vol));
    };

    syncSettingsUi();
    connect(m_settings, &SettingsModel::changed, this, syncSettingsUi);
    connect(m_ui->volumeSlider, &QSlider::valueChanged, m_settings, &SettingsModel::setVolume);

    m_ui->notificationWidget->setIdleMessage("Turn the cube and play music!");

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] { onEdgeTurned({});});
    timer->start(3000);
}

MainWindow::~MainWindow() = default;

void MainWindow::start()
{
    show();
}

void MainWindow::onEdgeTurned(const CubeEdge& edge)
{
    if(m_dialog)
        m_dialog->onEdgeTurned(edge);

    auto activePreset = m_presetModel->findPreset(m_presetModel->activePreset());
    if(!activePreset)
        return;

    auto harmony = activePreset->toHarmony(edge);
    m_ui->notificationWidget->notify(edge, harmony);
}

void MainWindow::onCreateNew()
{
    createPresetDialog();
    auto sourceName = m_presetModel->findVacantName("New preset");
    m_dialog->openCreatePresetPage(sourceName);
    m_dialog->exec();

    auto preset = m_dialog->currentPreset();

    if(!preset)
        return;

    auto name = m_presetModel->findVacantName(m_dialog->currentPresetName());
    m_presetModel->addPreset(name, preset);

    updatePresetPage();
}

void MainWindow::onOpenRequested(const QString &name)
{
    auto preset =  m_presetModel->findPreset(name);
    createPresetDialog();
    m_dialog->openEditPresetPage(name, preset);

    QScopedPointer<Preset::AbstractPreset::Backup> backup(preset->createBackup());

    if(m_dialog->exec() == QDialog::Rejected)
        backup->restore();

    updatePresetPage();
}

void MainWindow::showPresetListDialog()
{
    PresetListDialog dialog;
    dialog.setPresetList(m_presetModel->allPresets());
    auto res = dialog.exec();

    if(res == QDialog::Rejected)
        return;

    auto renamed = dialog.renamedPresets();
    for(auto oldName: renamed.keys())
    {
        auto newName = renamed.value(oldName);
        m_presetModel->renamePreset(oldName, newName);
    }

    for(auto name: dialog.removedPresets())
        m_presetModel->removePreset(name);
}

void MainWindow::createPresetDialog()
{
    if(!m_dialog)
        m_dialog.reset(new PresetDialog);
}

void MainWindow::updatePresetPage()
{
    if(!m_presetModel->allPresets().isEmpty())
        m_ui->stackedWidget->setCurrentWidget(m_ui->presetsPage);
    else
        m_ui->stackedWidget->setCurrentWidget(m_ui->noPresetPage);
}

void MainWindow::updateActionsState()
{
    auto isEmpty = m_presetModel->allPresets().isEmpty();

    m_ui->savePresetAction->setDisabled(isEmpty);
    m_ui->presetsListAction->setDisabled(isEmpty);
}

