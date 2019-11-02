#pragma once

#include <QMainWindow>
#include <QScopedPointer>

#include "src/CubeEdge.h"
#include "src/Model/Model.h"

namespace Ui {
class MainWindow;
}

class PresetDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model * model, QWidget *parent = nullptr);
    ~MainWindow();

    void start();

private slots:
    void onCreateNew();
    void onOpenRequested(const QString& name);
    void onEdgeTurned(const CubeEdge& edge);
    void showPresetListDialog();
    void createPresetDialog();

    void updatePresetPage();
    void updateActionsState();

private:
    QScopedPointer<Ui::MainWindow> m_ui;
    QScopedPointer<PresetDialog> m_dialog;
    PresetModel * m_presetModel = nullptr;
    SettingsModel * m_settings = nullptr;
};

