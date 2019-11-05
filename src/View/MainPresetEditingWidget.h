#pragma once

#include <QWidget>
#include "src/CubeEdge.h"

namespace Ui {
class MainPresetEditingWidget;
}

namespace Preset {
class AbstractPreset;
}

class QAbstractButton;
class QAction;
class QMenu;

class MainPresetEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainPresetEditingWidget(QWidget * parent = nullptr);
    ~MainPresetEditingWidget();

signals:
    void finished();

public slots:
    void setPreset(const QString& name, Preset::AbstractPreset * preset);
    void onEdgeTurned(const CubeEdge&);

private:
    QMap<QAbstractButton*, CubeEdge> selectors() const;
    QMenu * createMenu();

private:
    QScopedPointer<Ui::MainPresetEditingWidget> m_ui;
    QMap<QString, QAction*> m_actions;
};
