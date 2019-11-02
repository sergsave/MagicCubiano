#include "PresetDialog.h"
#include "ui_PresetDialog.h"

#include "src/Preset/Presets.h"

using namespace Preset;

PresetDialog::PresetDialog(QWidget * parent):
    QDialog(parent),
    m_ui(new Ui::PresetDialog)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::Window);

    m_ui->stackedWidget->setCurrentWidget(m_ui->creatingPage);

    connect(m_ui->creatingPage, &PresetCreatingWidget::presetCreated, this, [this](auto name, auto * preset){
        this->update(name, preset);
        this->openEditPresetPage(name, preset);
    });

    connect(m_ui->editingPage, &MainPresetEditingWidget::finished, this, &PresetDialog::accept);
}

PresetDialog::~PresetDialog() = default;

void PresetDialog::openCreatePresetPage(const QString &name)
{
    update(name, nullptr);
    m_ui->stackedWidget->setCurrentWidget(m_ui->creatingPage);
    m_ui->creatingPage->setPresetName(name);
}

void PresetDialog::openEditPresetPage(const QString &name, AbstractPreset *preset)
{
    update(name, preset);
    m_ui->stackedWidget->setCurrentWidget(m_ui->editingPage);
    m_ui->editingPage->setPreset(name, preset);
}

AbstractPreset *PresetDialog::currentPreset() const
{
    return m_preset;
}

QString PresetDialog::currentPresetName() const
{
    return m_name;
}

void PresetDialog::onEdgeTurned(const CubeEdge& ce)
{
    m_ui->editingPage->onEdgeTurned(ce);
}

void PresetDialog::update(const QString &name, AbstractPreset *preset)
{
    m_name = name;
    m_preset = preset;
}



