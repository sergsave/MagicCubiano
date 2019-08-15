#pragma once

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ~ConnectionDialog();

signals:
    void connectAnyRequested();
    void connectByAddressRequested(const QString&);

public slots:
    void connected();
    void connectionFailed();

private:
    enum Status
    {
        READY,
        IN_PROGRESS,
        FAILED
    };

    void setStatus(Status st);

private:
    QScopedPointer<Ui::ConnectionDialog> m_ui;
};

