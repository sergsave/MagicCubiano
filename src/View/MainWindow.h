#pragma once

#include <QWidget>
#include <QMap>
#include <QScopedPointer>

#include "EdgeWidget.h"

namespace Ui {
class MainWindow;
}

class ConnectionDialog;

// Now support only guitar mode
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void start();

    Music::Harmony harmonyFor(const CubeEdge& ) const;
    void highlightEdge(CubeEdge::Color col);

signals:
    void connectAnyRequested();
    void connectByAddressRequested(const QString&);

public slots:
    void connected();
    void connectionFailed();

private:
    void createEdgeWidgets();
    QList<EdgeWidget*> edgeWidgets();

    int defaultHarmonyDelayMsec() const;

private slots:
    void setRotationModeEnabled(bool en);

private:
    QMap<CubeEdge::Color, EdgeWidget*> m_color2edges;
    QScopedPointer<Ui::MainWindow> m_ui;

    ConnectionDialog * m_dialog;
};

