#pragma once

#include <QWidget>
#include <QScopedPointer>

namespace Preset {
class AbstractPreset;
}

namespace Ui {
class PresetCreatingWidget;
}

class PresetCreatingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PresetCreatingWidget(QWidget *parent = nullptr);
    ~PresetCreatingWidget();

    void setPresetName(const QString& name);

signals:
    void presetCreated(const QString& name, Preset::AbstractPreset *);

private:
    QScopedPointer<Ui::PresetCreatingWidget> m_ui;
};

