#include "ConnectingDialog.h"
#include "ui_ConnectingDialog.h"

ConnectingDialog::ConnectingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectingDialog)
{
    ui->setupUi(this);

    connect(ui->connectAnyButton, &QPushButton::clicked, this, [this] {
        emit connectAnyRequested();
        accept();
    });
    connect(ui->connectAddressButton, &QPushButton::clicked, this, [this] {
        // TODO: Validator
        emit connectByAddressRequested(ui->addressEdit->text());
        accept();
    });
}

ConnectingDialog::~ConnectingDialog()
{
    delete ui;
}
