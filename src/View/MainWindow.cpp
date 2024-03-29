#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopWidget>
#include <QMenu>

#include "src/Preset/Presets.h"
#include "PresetDialog.h"
#include "CubeStatusDialog.h"
#include "SaveLoadHelper.h"
#include "PresetListDialog.h"
#include "Utils.h"

MainWindow::MainWindow(Model * model, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_presetModel(model->presets())
    , m_settings(model->settings())
    , m_protocol(model->remoteProtocol())
{
    m_ui->setupUi(this);

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    createMenu();
    updatePresetPage();
    updateActionsState();

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetOpenRequested, this,
        &MainWindow::onOpenRequested);

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

    connect(m_protocol, &GiikerProtocol::cubeEdgeTurned, this, &MainWindow::onEdgeTurned);

    auto updateStatus = [this] {
        const bool isConnected = m_protocol->state() == GiikerProtocol::CONNECTED;

        QString statusLabelText = "Status: ";
        statusLabelText += isConnected ? "Connected" : "Disconnected";
        m_ui->cubeStatusLabel->setText(statusLabelText);

        const QString notificationIdleText = isConnected ?
                    "Turn the cube and play music!" :
                    "Connect the cube for playing music!";

        m_ui->notificationWidget->setIdleMessage(notificationIdleText);
    };

    updateStatus();

    connect(m_protocol, &GiikerProtocol::connected, this, updateStatus);
    connect(m_protocol, &GiikerProtocol::disconnected, this, updateStatus);
}

MainWindow::~MainWindow() = default;

void MainWindow::start()
{
    showStatusDialog(true);
    show();
}

void MainWindow::createMenu()
{
    auto menu = new QMenu(this);

    menu->addSeparator();
    auto newPresetAction = menu->addAction("New Preset");
    auto loadPresetAction = menu->addAction("Load Preset");
    auto loadAllPresetsAction = menu->addAction("Load Presets");
    auto savePresetAction = menu->addAction("Save Preset");
    menu->addSeparator();
    auto presetsListAction = menu->addAction("Presets list");
    menu->addSeparator();
    auto cubeStatusAction = menu->addAction("Cube Status");

    // Use custom instead QMenuBar. QMenuBar is looking terrible on Android
    connect(m_ui->createNewButton, &QAbstractButton::clicked, newPresetAction, &QAction::trigger);
    connect(m_ui->loadButton, &QAbstractButton::clicked, loadPresetAction, &QAction::trigger);
    connect(m_ui->loadAllButton, &QAbstractButton::clicked, loadAllPresetsAction, &QAction::trigger);

    auto saveLoadHelper = new SaveLoadHelper(this);
    connect(newPresetAction, &QAction::triggered, this, &MainWindow::onCreateNew);
    connect(loadPresetAction, &QAction::triggered, saveLoadHelper, &SaveLoadHelper::choosePathForLoading);
    connect(loadAllPresetsAction, &QAction::triggered, saveLoadHelper,
        &SaveLoadHelper::choosePathForLoadingAll);
    connect(savePresetAction, &QAction::triggered, this, [saveLoadHelper, this] {
        saveLoadHelper->choosePathForSaving(m_presetModel->activePreset());
    });
    connect(presetsListAction, &QAction::triggered, this, &MainWindow::showPresetListDialog);
    connect(cubeStatusAction, &QAction::triggered, this, &MainWindow::showStatusDialog);

    // TODO: Handle save-load errors
    connect(saveLoadHelper, &SaveLoadHelper::loadRequested, m_presetModel, &PresetModel::loadPreset);
    connect(saveLoadHelper, &SaveLoadHelper::loadAllRequested, m_presetModel, &PresetModel::loadAllPresets);
    connect(saveLoadHelper, &SaveLoadHelper::saveRequested, m_presetModel, &PresetModel::savePresets);

    m_nonEmptyPresetsActions << savePresetAction << presetsListAction;

    bindMenu(menu, m_ui->menuButton);
}

void MainWindow::onEdgeTurned(const CubeEdge& edge)
{
    if(m_presetDialog)
        m_presetDialog->onEdgeTurned(edge);

    auto activePreset = m_presetModel->findPreset(m_presetModel->activePreset());
    if(!activePreset)
        return;

    auto harmony = activePreset->toHarmony(edge);
    m_ui->notificationWidget->notify(edge, harmony);
}

void MainWindow::createPresetDialog()
{
    m_presetDialog.reset(new PresetDialog);
}

void MainWindow::onCreateNew()
{
    createPresetDialog();
    auto sourceName = m_presetModel->findVacantName("New preset");
    m_presetDialog->openCreatePresetPage(sourceName);

    connect(m_presetDialog.data(), &PresetDialog::presetCreated, this, [this](auto name, auto preset) {
        if(!preset)
            return;

        auto vacName = m_presetModel->findVacantName(name);
        m_presetModel->addPreset(vacName, preset);

        this->updatePresetPage();
    });
    m_presetDialog->exec();
}

void MainWindow::onOpenRequested(const QString &name)
{
    auto preset =  m_presetModel->findPreset(name);
    createPresetDialog();
    m_presetDialog->openEditPresetPage(name, preset);

    QScopedPointer<Preset::AbstractPreset::Backup> backup(preset->createBackup());

    if(m_presetDialog->exec() == QDialog::Rejected)
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

void MainWindow::updatePresetPage()
{
    if(!m_presetModel->allPresets().isEmpty())
        m_ui->stackedWidget->setCurrentWidget(m_ui->presetsPage);
    else
        m_ui->stackedWidget->setCurrentWidget(m_ui->noPresetPage);
}

void MainWindow::updateActionsState()
{
    for(auto a : m_nonEmptyPresetsActions)
        a->setDisabled(m_presetModel->allPresets().isEmpty());
}

void MainWindow::showStatusDialog(bool closeOnConnect)
{
    CubeStatusDialog dialog;

    auto updatePage = [&dialog, this] {
        if(m_protocol && m_protocol->state() == GiikerProtocol::CONNECTED)
            dialog.goToConnectedPage();
        else
            dialog.goToDisconnectedPage();
    };

    connect(&dialog, &CubeStatusDialog::connectAnyRequested, m_protocol, &GiikerProtocol::connectToCube);
    connect(&dialog, &CubeStatusDialog::connectByAddressRequested, m_protocol,
            &GiikerProtocol::connectToCubeByAddress);
    connect(&dialog, &CubeStatusDialog::batteryLevelRequested, m_protocol,
            &GiikerProtocol::requestBatteryLevel);
    connect(&dialog, &CubeStatusDialog::disconnectRequested, m_protocol, &GiikerProtocol::disconnectFromCube);
    connect(&dialog, &CubeStatusDialog::rejected, m_protocol, &GiikerProtocol::cancelConnection);

    connect(m_protocol, &GiikerProtocol::connected, &dialog, updatePage);
    connect(m_protocol, &GiikerProtocol::connectionFailed, &dialog, &CubeStatusDialog::onConnectionFailed);
    connect(m_protocol, &GiikerProtocol::disconnected, &dialog, updatePage);
    connect(m_protocol, &GiikerProtocol::batteryLevelResponsed, &dialog, &CubeStatusDialog::setBatteryLevel);

    if(closeOnConnect)
        connect(m_protocol, &GiikerProtocol::connected, &dialog, &CubeStatusDialog::accept);

    updatePage();

    dialog.exec();
}

