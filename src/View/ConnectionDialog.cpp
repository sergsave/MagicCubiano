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
    // Set stylesheet causes to dialog flikking. Use HTML formatting instead.
    QString message;

    switch (st)
    {
    case READY:
        message = "<span>Find your cube<br> </span>";
        break;
    case IN_PROGRESS:
        message = "<span>Connecting...<br> </span>";
        break;
    case FAILED:
        message = "<span style=\" color:red;\">Failed!<br>Check the Bluetooth.</span>";
        break;
    default:
        assert(!"Unknow!");
        break;
    }

    m_ui->statusLabel->setText(message);
}

ConnectionDialog::~ConnectionDialog() = default;
