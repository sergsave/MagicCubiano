#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMap>

#include "CubeEdge.h"
#include "GuitarFretboardPos.h"

namespace Ui {
class MainWindow;
}
class EdgeSettingsWidget;

// Now support only guitar mode
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // TODO: Add abstract music info, and specific types for piano, notes etc.
    void setMaxGuitarFretboardPos(const GuitarFretboardPos&);
    GuitarFretboardPos guitarFretboardPosFor(const CubeEdge& ) const;

    int soundDuration() const;

public slots:
    void setStatus(const QString &text);

private slots:
    void groupStringModeToggled(bool st);
    void bedirectModeToggled(bool st);

private:
    void setEdgeWidgetsColor();

private:
    QMap<CubeEdge::Color, EdgeSettingsWidget*> m_color2edges;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
