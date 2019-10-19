#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopWidget>

#include "src/Preset/Storage.h"
#include "PresetDialog.h"
#include "Utils.h"

MainWindow::MainWindow(Preset::Storage *storage, QWidget *parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_storage(storage)
{
    m_ui->setupUi(this);

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    updatePresetPage();

    connect(m_ui->createNewButton, &QAbstractButton::clicked, this, &MainWindow::onCreateNew);
    connect(m_ui->testButton, &QAbstractButton::clicked, this, &MainWindow::onCreateNew);

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetEditRequested, this,
            &MainWindow::onEditRequested);

    connect(m_ui->presetSelectionWidget, &PresetSelectionWidget::presetRenamed, this,
            [this] (auto oldName, auto newName) {
        m_storage->renamePreset(oldName, newName);
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
    m_ui->presetSelectionWidget->add(name, instrumentName(preset));

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

