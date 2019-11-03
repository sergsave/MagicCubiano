#pragma once

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
class CubeStatusDialog;
}

class CubeStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CubeStatusDialog(QWidget *parent = 0);
    ~CubeStatusDialog();

signals:
    void connectAnyRequested();
    void connectByAddressRequested(const QString&);
    void batteryLevelRequested();

public slots:
    void goToConnectedPage();
    void goToDisconnectedPage();
    void onConnectionFailed();
    void setBatteryLevel(int batteryInPerc);

private:
    static const int m_batteryPollingTimeout = 5000;

    enum Page
    {
        CONNECTED,
        DISCONNECTED
    };
    void setPage(Page);

    enum ConnectionStatus
    {
        READY,
        IN_PROGRESS,
        FAILED
    };
    void setConnectionStatus(ConnectionStatus);

    QString getAddresFromInputDialog();

private:
    QScopedPointer<Ui::CubeStatusDialog> m_ui;
};

