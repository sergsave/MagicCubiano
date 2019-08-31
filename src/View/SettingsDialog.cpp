#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(const Settings &sett, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog),
    m_settings(sett)
{
    m_ui->setupUi(this);
    m_ui->durationLineEdit->setValue(m_settings.delayMSec);
}

Settings SettingsDialog::settings() const
{
    return { m_ui->durationLineEdit->value() };
}

SettingsDialog::~SettingsDialog() = default;
