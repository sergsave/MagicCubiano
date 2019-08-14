#ifndef CONNECTINGDIALOG_H
#define CONNECTINGDIALOG_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
class ConnectingDialog;
}

class ConnectingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectingDialog(QWidget *parent = 0);
    ~ConnectingDialog();

signals:
    void connectAnyRequested();
    void connectByAddressRequested(const QString&);

private:
    QScopedPointer<Ui::ConnectingDialog> m_ui;
};

#endif // CONNECTINGDIALOG_H
