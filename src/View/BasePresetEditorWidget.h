#pragma once

#include <QWidget>
#include "src/CubeEdge.h"

class BasePresetEditorWidget : public QWidget
{
    Q_OBJECT
public:
    using QWidget::QWidget;
    virtual ~BasePresetEditorWidget() {}

    void setActiveCubeEdge(const CubeEdge& edge) { m_edge = edge; }
    CubeEdge activeCubeEdge() const { return m_edge; }

private:
    CubeEdge m_edge;
};
