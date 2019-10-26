#pragma once

#include <QMainWindow>
#include <QScopedPointer>

#include "src/CubeEdge.h"

namespace Ui {
class MainWindow;
}

namespace Preset {
class Model;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Preset::Model * model, QWidget *parent = nullptr);
    ~MainWindow();

    void start();

    void onEdgeTurned(const CubeEdge& edge);

private slots:
    void onCreateNew();
    void onOpenRequested(const QString& name);
    void showPresetListDialog();

    void updatePresetPage();
    void updateActionsState();

private:
    QScopedPointer<Ui::MainWindow> m_ui;
    Preset::Model * m_model = nullptr;
};

