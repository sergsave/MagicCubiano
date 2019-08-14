#ifndef EDGESETTINGS_H
#define EDGESETTINGS_H

#include <QWidget>
#include <QScopedPointer>
#include <functional>

#include "MagicCubianoGlobal.h"

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

    void setEdgeColor(CubeEdge::Color color);
    CubeEdge::Color edgeColor() const;

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

    void forEachFretSliders(std::function<void(QSlider *)>);
    void forEachStringSliders(std::function<void(QSlider *)>);

    void updateButtonIcon();

private:
    CubeEdge::Rotation m_rotationPage = CubeEdge::CLOCKWIZE;
    CubeEdge::Color m_color = CubeEdge::GREEN;

    bool m_rotationMode = false;
    QScopedPointer<Ui::EdgeSettingsWidget> m_ui;
};

#endif // EDGESETTINGS_H
