#pragma once

#include <QWidget>

#include "src/Preset/Presets.h"

class GuitarStringWidget;

class GuitarNotationsWidget : public QWidget
{
    Q_OBJECT
public:
    using Notations = QList<Instruments::GuitarNotation>;

    explicit GuitarNotationsWidget(const Instruments::GuitarNotation& min,
                                   const Instruments::GuitarNotation& max,
                                   QWidget * parent);
    ~GuitarNotationsWidget();

    void setNotations(const Notations& notations);
    Notations notations();

signals:
    void notationsChanged(const Notations& notations);

private:
    void onFretChanged(int string, int fret);
    void onMuteChanged(int string, bool mute);
    GuitarStringWidget * stringWidget(int string);

private:
    QList<GuitarStringWidget*> m_widgets;
};
