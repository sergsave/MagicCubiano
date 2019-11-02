#pragma once

#include <QDialog>
#include <QScopedPointer>

#include "src/CubeEdge.h"

namespace Ui {
class PresetDialog;
}

namespace Preset {
class AbstractPreset;
}

class PresetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PresetDialog(QWidget * parent = nullptr);
    ~PresetDialog();

    void openCreatePresetPage(const QString& name);
    void openEditPresetPage(const QString& name, Preset::AbstractPreset * preset);

    // Preset dialog doesn't take ownership of preset
    Preset::AbstractPreset * currentPreset() const;
    QString currentPresetName() const;

public slots:
    void onEdgeTurned(const CubeEdge&);

private:
    void update(const QString& name, Preset::AbstractPreset * preset);

private:
    QScopedPointer<Ui::PresetDialog> m_ui;
    Preset::AbstractPreset * m_preset = nullptr;
    QString m_name;
};
