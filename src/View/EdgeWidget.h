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

    CubeEdge::Color edgeColor() const;
    void setEdgeColor(CubeEdge::Color color);

    Music::Harmony harmony(CubeEdge::Rotation dir) const;
    void setHarmony(const Music::Harmony& harmony, CubeEdge::Rotation dir);

    CubeEdge::Rotation rotateDirection() const;
    void setRotateDirection(CubeEdge::Rotation dir);

private:
    void updateSettingsButtons();
    void updateRotationButton();
    void enterSettings();
    void updateSettingsIcon(CubeEdge::Rotation rot);

    QString rotIconPath() const;
    EdgeSettingsDialog * settings(CubeEdge::Rotation) const;

    void showEvent(QShowEvent *event) override;

private:
    CubeEdge::Color m_color = CubeEdge::GREEN;
    CubeEdge::Rotation m_rotateDir = CubeEdge::CLOCKWIZE;

    QMap<CubeEdge::Rotation, EdgeSettingsDialog*> m_rotation2settings;
    QScopedPointer<Ui::EdgeWidget> m_ui;
};

