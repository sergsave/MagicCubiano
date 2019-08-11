#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "CubeEdgeType.h"
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
    GuitarFretboardPos guitarFretboardPos(EdgeType type) const;

    int soundDuration() const;

public slots:
    void setStatus(const QString &text);

private:
    void setEdgeWidgetsName();
    EdgeSettingsWidget * edgeWidget(EdgeType type) const;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
