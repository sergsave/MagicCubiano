#pragma once

#include <QMainWindow>
#include <QScopedPointer>

#include "src/CubeEdge.h"

namespace Ui {
class MainWindow;
}

namespace Preset {
class Storage;
class AbstractPreset;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Preset::Storage * storage, QWidget *parent = nullptr);
    ~MainWindow();

    void start();

    void onEdgeTurned(const CubeEdge& edge);


private slots:
    void onCreateNew();
    void onEditRequested(const QString& name);
    void updatePresetPage();
    void addToSelectionWidget(const QString& name);

private:
    QScopedPointer<Ui::MainWindow> m_ui;
    Preset::Storage * m_storage = nullptr;
};

