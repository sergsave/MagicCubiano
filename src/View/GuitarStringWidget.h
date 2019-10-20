#pragma once

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
class GuitarStringWidget;
}

class GuitarStringWidget: public QWidget
{
    Q_OBJECT

public:
    GuitarStringWidget(int number, int minFret, int maxFret, QWidget * parent);

    ~GuitarStringWidget();

    int number() const;

    bool isMuted() const;
    void setMuted(bool st);

    int fretValue() const;
    void setFretValue(int val);

    void reset();

signals:
    void fretValueChanged(int);
    void muteChaged(bool);

private:
    QScopedPointer<Ui::GuitarStringWidget> m_ui;
};
