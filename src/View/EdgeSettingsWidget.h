#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QColor>

#include <functional>

#include "src/MagicCubianoGlobal.h"

namespace Ui {
class EdgeSettingsWidget;
}
class QSlider;

class EdgeSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdgeSettingsWidget(QWidget * parent = 0);
    ~EdgeSettingsWidget();

    void blinkEdgeButton();

    void setEdgeButtonColor(CubeEdge::Color color);
    CubeEdge::Color edgeButtonColor() const;

    // In rotation mode clockwize and anticlockwize slider are not synchronized
    void setRotationModeEnabled(bool);
    bool inRotationMode(bool) const;

    void setMaxStringNumber(int );
    void setMaxFretNumber(int );

    void setMinStringNumber(int );
    void setMinFretNumber(int );

    int stringFor(CubeEdge::Rotation rot) const;
    int fretFor(CubeEdge::Rotation rot) const;

public slots:
    void setCurrentString(int);
    void setCurrentFret(int);

signals:
    void stringChanged(int);
    void fretChanged(int);

private:
    void setRotationPage(CubeEdge::Rotation rot);

    QSlider * currFretSlider() const;
    QSlider * currStringSlider() const;

    void forEachFretSlider(std::function<void(QSlider *)>);
    void forEachStringSlider(std::function<void(QSlider *)>);

    void updateButtonIcon();

private:
    CubeEdge::Rotation m_rotationPage = CubeEdge::CLOCKWIZE;
    CubeEdge::Color m_color = CubeEdge::GREEN;

    bool m_rotationMode = false;
    QScopedPointer<Ui::EdgeSettingsWidget> m_ui;
};

