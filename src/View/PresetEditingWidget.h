#pragma once

#include <QWidget>
#include "src/CubeEdge.h"

namespace Ui {
class PresetEditingWidget;
}

namespace Preset {
class AbstractPreset;
}

class BasePresetEditorWidget;
class QAbstractButton;

class PresetEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PresetEditingWidget(QWidget * parent = nullptr);
    ~PresetEditingWidget();

signals:
    void finished();

public slots:
    void setPreset(const QString& name, Preset::AbstractPreset * preset);

private:
    void setEditorWidget(BasePresetEditorWidget*);
    QMap<QAbstractButton*, CubeEdge> selectors() const;

private:
    QScopedPointer<Ui::PresetEditingWidget> m_ui;
    BasePresetEditorWidget * m_editor = nullptr;
};
