#pragma once

#include <QDialog>
#include <QScopedPointer>

#include "src/Configuration.h"

namespace Ui {
class SettingsDialog;
}

struct Settings
{
    Settings(int msec = Configuration::defaultMinNoteDurationMSec(), int vol = 100) :
        delayMSec(msec),
        volume(vol)
    {}
    int delayMSec;
    int volume;
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
};

