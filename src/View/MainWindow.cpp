#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ConnectionDialog.h"

#include <QScopedPointer>
#include <QDesktopWidget>

#include <cassert>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    createEdgeWidgets();

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    connect(m_ui->bidrectCheckBox, &QAbstractButton::toggled, this, &MainWindow::setRotationModeEnabled);
}

MainWindow::~MainWindow() = default;

void MainWindow::start()
{
    m_dialog = new ConnectionDialog(this);

    connect(m_dialog, &ConnectionDialog::connectAnyRequested, this, &MainWindow::connectAnyRequested);
    connect(m_dialog, &ConnectionDialog::connectByAddressRequested, this, &MainWindow::connectByAddressRequested);
    m_dialog->exec();

    show();
}

Music::Harmony MainWindow::harmonyFor(const CubeEdge & edge) const
{
    auto edgeWidget = m_color2edges.value(edge.color, nullptr);

    if(!edgeWidget)
        return {};

    auto harmony = edgeWidget->harmonyFor(edge.rotation);

    if(harmony.delayMSec == 0)
        harmony.delayMSec = defaultHarmonyDelayMsec();

    return harmony;
}

void MainWindow::connected()
{
    if(m_dialog) m_dialog->connected();
}

void MainWindow::connectionFailed()
{
    if(m_dialog) m_dialog->connectionFailed();
}

void MainWindow::createEdgeWidgets()
{
    using Col = CubeEdge::Color;
    qDeleteAll(m_color2edges);
    m_color2edges.clear();

    auto layout = m_ui->edgesFrame->layout();
    if(!layout)
        layout = new QVBoxLayout(m_ui->edgesFrame);

    auto factory = new GuitarEdgeSettingsFactory;

    auto addWidget = [layout, factory, this] (CubeEdge::Color col) {
        auto w = new EdgeWidget(col, factory, this);
        m_color2edges[col] = w;
        layout->addWidget(w);
    };

    addWidget(Col::GREEN);
    addWidget(Col::BLUE);
    addWidget(Col::RED);
    addWidget(Col::ORANGE);
    addWidget(Col::WHITE);
    addWidget(Col::YELLOW);
}

QList<EdgeWidget *> MainWindow::edgeWidgets()
{
    assert(!m_color2edges.empty());
    return m_color2edges.values();
}

int MainWindow::defaultHarmonyDelayMsec() const
{
    return m_ui->durLineEdit->text().toInt();
}

void MainWindow::setRotationModeEnabled(bool en)
{
    for(auto ew: edgeWidgets())
        ew->setRotationModeEnabled(en);
}

void MainWindow::highlightEdge(CubeEdge::Color color)
{
    if(auto edge = m_color2edges.value(color, nullptr))
        edge->indicate();
}
