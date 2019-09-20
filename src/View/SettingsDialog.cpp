#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(const Settings &sett, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);
    m_ui->delaySpinBox->setValue(sett.delayMSec);
    m_ui->volumeSpinBox->setValue(sett.volume);
}

Settings SettingsDialog::settings() const
{
    auto delay = m_ui->delaySpinBox->value();
    auto volume = m_ui->volumeSpinBox->value();

    return { delay, volume };
}

SettingsDialog::~SettingsDialog() = default;
