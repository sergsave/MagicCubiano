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

    connect(m_ui->syncButton, &QAbstractButton::clicked, this, &MainWindow::synchronizeEdgesRotation);
    connect(m_ui->resetButton, &QAbstractButton::clicked, this, &MainWindow::setDefaultHarmonies);
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

    auto harmony = edgeWidget->harmony(edge.rotation);

    if(harmony.delayMSec == 0)
        harmony.delayMSec = defaultHarmonyDelayMsec();

    return harmony;
}

void MainWindow::highlightEdge(CubeEdge::Color color)
{
    if(auto edge = m_color2edges.value(color, nullptr))
        edge->indicate();
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

    assert(m_color2edges.empty());

    auto layout = m_ui->edgesFrame->layout();
    if(!layout)
        layout = new QVBoxLayout(m_ui->edgesFrame);

    QScopedPointer<EdgeSettingsFactory> factory(new GuitarEdgeSettingsFactory);

    auto addWidget = [layout, &factory, this] (CubeEdge::Color col) {
        auto w = new EdgeWidget(col, factory.data(), this);
        layout->addWidget(w);

        m_color2edges[col] = w;
    };

    addWidget(Col::YELLOW);
    addWidget(Col::ORANGE);
    addWidget(Col::RED);
    addWidget(Col::GREEN);
    addWidget(Col::WHITE);
    addWidget(Col::BLUE);

    setDefaultHarmonies();
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

void MainWindow::setAllDirectionHarmony(EdgeWidget *ew, const Music::Harmony& harm)
{
    ew->setHarmony(harm, CubeEdge::Rotation::ANTICLOCKWIZE);
    ew->setHarmony(harm, CubeEdge::Rotation::CLOCKWIZE);
}

void MainWindow::synchronizeEdgesRotation()
{
    for(auto ew: edgeWidgets())
    {
        if(!ew) continue;
        ew->indicate();

        auto currHarmony = ew->harmony(ew->rotateDirection());
        setAllDirectionHarmony(ew, currHarmony);
    }
}

void MainWindow::setDefaultHarmonies()
{
    for(auto ew: edgeWidgets())
    {
        if(!ew) continue;
        ew->indicate();

        using namespace Music;

        auto tone = Tone(Tone::E, 2);
        auto delay = defaultHarmonyDelayMsec();
        auto harmony = Harmony({tone}, delay);

        setAllDirectionHarmony(ew, harmony);
    }
}
