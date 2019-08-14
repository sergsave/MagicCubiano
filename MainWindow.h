#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMap>
#include <QScopedPointer>

#include "MagicCubianoGlobal.h"
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

    void start();

    // TODO: Add abstract music info, and specific types for piano, notes etc.
    void setMaxGuitarFretboardPos(const GuitarFretboardPos&);
    GuitarFretboardPos guitarFretboardPosFor(const CubeEdge& ) const;

    int soundDuration() const;

signals:
    void connectAnyRequested();
    void connectByAddressRequested(const QString&);

public slots:
    void setStatus(const QString &text);

private slots:
    void groupStringModeToggled(bool st);
    void bedirectModeToggled(bool st);

private:
    void initEdgeWidgets();
    QList<EdgeSettingsWidget*> edgeWidgets();

private:
    QMap<CubeEdge::Color, EdgeSettingsWidget*> m_color2edges;
    QScopedPointer<Ui::MainWindow> m_ui;
};

#endif // MAINWINDOW_H
