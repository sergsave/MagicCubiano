#pragma once

#include <QDialog>
#include <QScopedPointer>

#include "src/Configuration.h"

namespace Ui {
class SettingsDialog;
}

struct Settings
{
    Settings(int msec = Configuration::noteDurationMSec()) : delayMSec(msec) {}
    int delayMSec;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const Settings& sett, QWidget * parent = nullptr);
    ~SettingsDialog();

    Settings settings() const;

private:
    QScopedPointer<Ui::SettingsDialog> m_ui;
    Settings m_settings;
};

