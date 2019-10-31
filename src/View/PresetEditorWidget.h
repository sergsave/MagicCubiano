#pragma once

#include <QWidget>
#include <QScopedPointer>

#include "src/CubeEdge.h"
#include "src/Preset/Presets.h"
#include <functional>

namespace Ui {
class PresetEditorWidget;
}

namespace Preset {
class AbstractPreset;
}

class PresetEditorWidget : public QWidget
{
    Q_OBJECT
public:
    PresetEditorWidget(QWidget * parent = nullptr);
    virtual ~PresetEditorWidget();

public slots:
    void resetData();
    void syncDataByClockwize();
    void syncDataByAnticlockwize();
    void setAdditionsVisible(bool visible);

    void setActiveCubeEdge(const CubeEdge&);
    void setPreset(Preset::AbstractPreset *);

public:
    CubeEdge activeCubeEdge() const;

private:
    void createEditor(Preset::AbstractPreset *);
    void updateEditor();
    void updateAdditions();
    void updateAll();

private:
    QScopedPointer<Ui::PresetEditorWidget> m_ui;
    Preset::AbstractPreset * m_preset = nullptr;
    QWidget * m_editor = nullptr;

    std::function<void()> m_updateFunctor;
    CubeEdge m_edge;
};
