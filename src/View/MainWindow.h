#pragma once

#include <QMainWindow>
#include <QScopedPointer>
#include <QSet>

#include "src/CubeEdge.h"
#include "src/Model/Model.h"

namespace Ui {
class MainWindow;
}

class PresetDialog;
class CubeStatusDialog;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model * model, QWidget *parent = nullptr);
    ~MainWindow();

    void start();

private:
    void createMenu();

private slots:
    void onCreateNew();
    void onOpenRequested(const QString& name);
    void onEdgeTurned(const CubeEdge& edge);
    void showPresetListDialog();
    void createPresetDialog();

    void updatePresetPage();
    void updateActionsState();
    void showStatusDialog(bool closeOnConnect = false);

private:
    QScopedPointer<Ui::MainWindow> m_ui;
    QScopedPointer<PresetDialog> m_presetDialog;
    PresetModel * m_presetModel = nullptr;
    SettingsModel * m_settings = nullptr;
    GiikerProtocol * m_protocol = nullptr;
    QSet<QAction*> m_nonEmptyPresetsActions;
};

