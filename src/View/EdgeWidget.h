#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QMap>

#include "src/CubeEdge.h"
#include "src/Music.h"
#include "EdgeSettingsFactory.h"

namespace Ui {
class EdgeWidget;
}

class EdgeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdgeWidget(CubeEdge::Color color,
                        EdgeSettingsFactory* factory,
                        QWidget * parent = nullptr);
    ~EdgeWidget();

    void indicate();

    void setEdgeColor(CubeEdge::Color color);
    CubeEdge::Color edgeColor() const;

    void setRotationModeEnabled(bool);
    bool inRotationMode(bool) const;

    Music::Harmony harmonyFor(CubeEdge::Rotation) const;

private:
    void setRotationPage(CubeEdge::Rotation rot);
    void updateRotationButton();
    void updateSettingsButtons();
    void enterSettings();

    Music::Harmony currentHarmony() const;

private:
    CubeEdge::Color m_color = CubeEdge::GREEN;

    CubeEdge::Rotation m_rotationPage = CubeEdge::CLOCKWIZE;
    bool m_rotationMode = false;
    QMap<CubeEdge::Rotation, EdgeSettingsDialog*> m_rotation2settings;

    QScopedPointer<Ui::EdgeWidget> m_ui;
};

