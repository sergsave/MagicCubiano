#include "ConnectionDialog.h"
#include "ui_ConnectionDialog.h"

#include <cassert>

ConnectionDialog::ConnectionDialog(QWidget * parent) :
    QDialog(parent),
    m_ui(new Ui::ConnectionDialog)
{
    m_ui->setupUi(this);

    connect(m_ui->connectAnyButton, &QPushButton::clicked, this, [this] {
        setStatus(IN_PROGRESS);
        emit connectAnyRequested();
    });
    connect(m_ui->connectAddressButton, &QPushButton::clicked, this, [this] {
        // TODO: Validator
        setStatus(IN_PROGRESS);
        emit connectByAddressRequested(m_ui->addressEdit->text());
    });

    setStatus(READY);
}

void ConnectionDialog::connected()
{
    accept();
}

void ConnectionDialog::connectionFailed()
{
    setStatus(FAILED);
}

void ConnectionDialog::setStatus(ConnectionDialog::Status st)
{
    QString msg;
    QString style;
    switch (st) {
    case READY:
        style = "color: black";
        msg = "Find your cube \n";
        break;
    case IN_PROGRESS:
        style = "color: black";
        msg = "Connecting...\n";
        break;
    case FAILED:
        style = "color: red";
        msg = "Failed.\n Turn on the bluetooth!";
        break;
    default:
        assert("Unknow!");
        break;
    }

    m_ui->statusLabel->setStyleSheet(style);
    m_ui->statusLabel->setText(msg);
}

ConnectionDialog::~ConnectionDialog() = default;
