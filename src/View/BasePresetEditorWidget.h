#pragma once

#include <QWidget>
#include "src/CubeEdge.h"

class BasePresetEditorWidget : public QWidget
{
    Q_OBJECT
public:
    using QWidget::QWidget;
    virtual ~BasePresetEditorWidget() {}

public slots:
    virtual void resetData() = 0;
    virtual void syncDataByClockwize() = 0;
    virtual void syncDataByAnticlockwize() = 0;

    void setActiveCubeEdge(const CubeEdge& edge)
    {
        m_edge = edge;
        onCubeEdgeChanged(edge);
    }

public:
    CubeEdge activeCubeEdge() const { return m_edge; }

protected:
    virtual void onCubeEdgeChanged(const CubeEdge&) = 0;

private:
    CubeEdge m_edge;
};
