#pragma once

#include <QWidget>
#include <QMap>
#include <QScopedPointer>

#include "EdgeWidget.h"
#include "SettingsDialog.h"

namespace Ui {
class MainWindow;
}

class ConnectionDialog;

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
    void instrumentTypeChanged(Music::Instrument);

public slots:
    void connected();
    void connectionFailed();

private:
    void createEdgeWidgets();
    QList<EdgeWidget*> edgeWidgets();

    void createSettingsFactory();
    void updateSettingsFactory();

    void setAllDirectionHarmony(EdgeWidget *, const Music::Harmony &);
    int harmonyDelayMsec() const;

private slots:
    void synchronizeEdgesRotation();
    void setDefaultHarmonies();
    void onInstrumentTypeChanged(Music::Instrument);
    void enterGlobalSettings();

private:
    QMap<CubeEdge::Color, EdgeWidget*> m_color2edges;
    QScopedPointer<const EdgeSettingsFactory> m_settingsFactory;

    QScopedPointer<Ui::MainWindow> m_ui;
    ConnectionDialog * m_dialog = nullptr;
    Settings m_globalSettings;
};

