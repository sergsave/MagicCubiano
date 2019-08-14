#include "ConnectingDialog.h"
#include "ui_ConnectingDialog.h"

ConnectingDialog::ConnectingDialog(QWidget * parent) :
    QDialog(parent),
    m_ui(new Ui::ConnectingDialog)
{
    m_ui->setupUi(this);

    connect(m_ui->connectAnyButton, &QPushButton::clicked, this, [this] {
        emit connectAnyRequested();
        accept();
    });
    connect(m_ui->connectAddressButton, &QPushButton::clicked, this, [this] {
        // TODO: Validator
        emit connectByAddressRequested(m_ui->addressEdit->text());
        accept();
    });
}

ConnectingDialog::~ConnectingDialog() = default;
