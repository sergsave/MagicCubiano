#include "CubeStatusDialog.h"
#include "ui_CubeStatusDialog.h"

#include <QTimer>
#include <QInputDialog>
#include <QLineEdit>

#include <cassert>

CubeStatusDialog::CubeStatusDialog(QWidget * parent) :
    QDialog(parent),
    m_ui(new Ui::CubeStatusDialog)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::Window);

    connect(m_ui->connectToAnyButton, &QPushButton::clicked, this, [this] {
        setConnectionStatus(IN_PROGRESS);
        emit connectAnyRequested();
    });

    connect(m_ui->connectByAddressButton, &QPushButton::clicked, this, [this] {
        auto addr = getAddresFromInputDialog();
        if(!addr.isEmpty())
        {
            setConnectionStatus(IN_PROGRESS);
            emit connectByAddressRequested(addr);
        }
    });

    connect(m_ui->disconnectButton, &QPushButton::clicked, this, [this] {
        setConnectionStatus(READY);
        emit disconnectRequested();
    });

    connect(m_ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    goToDisconnectedPage();
    setConnectionStatus(READY);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CubeStatusDialog::batteryLevelRequested);
    timer->start(m_batteryPollingTimeout);
}

void CubeStatusDialog::goToConnectedPage()
{
    setPage(CONNECTED);
    emit batteryLevelRequested();
}

void CubeStatusDialog::goToDisconnectedPage()
{
    setPage(DISCONNECTED);
}

void CubeStatusDialog::setBatteryLevel(int batteryInPerc)
{
    m_ui->batteryValueLabel->setText(QString("%1%").arg(batteryInPerc));
}

void CubeStatusDialog::onConnectionFailed()
{
    setConnectionStatus(FAILED);
}

void CubeStatusDialog::setPage(CubeStatusDialog::Page page)
{
    QWidget * widget = nullptr;
    if(page == CubeStatusDialog::CONNECTED)
        widget = m_ui->connectionPage;
    else
        widget = m_ui->disconnectionPage;

    m_ui->stackedWidget->setCurrentWidget(widget);
}

void CubeStatusDialog::setConnectionStatus(CubeStatusDialog::ConnectionStatus st)
{
    // Set stylesheet causes to dialog flikking. Use HTML formatting instead.
    QString message;

    switch (st)
    {
    case READY:
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

    m_ui->connectionStatusLabel->setText(message);
}

QString CubeStatusDialog::getAddresFromInputDialog()
{
    QInputDialog dialog(this);
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setLabelText("Enter a MAC-Address");
    dialog.setTextValue("FE:A3:CB:06:AC:01");

    // Hack, but don't wanna create custom dialog
    auto lineEdit = dialog.findChild<QLineEdit*>();
    lineEdit->setInputMask("HH:HH:HH:HH:HH:HH;_");

    dialog.setStyleSheet("QDialog { border: 2px solid grey }");

    if(dialog.exec() == QDialog::Rejected)
        return {};

    return dialog.textValue();
}

CubeStatusDialog::~CubeStatusDialog() = default;
