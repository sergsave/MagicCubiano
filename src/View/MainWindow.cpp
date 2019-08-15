#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ConnectionDialog.h"

#include <QScopedPointer>

#include <cassert>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    initEdgeWidgets();

    connect(m_ui->bidrectCheckBox, &QCheckBox::toggled, this, &MainWindow::bedirectModeToggled);
    connect(m_ui->groupStringCheckBox, &QCheckBox::toggled, this, &MainWindow::groupStringModeToggled);

    setMaxGuitarFretboardPos({GuitarFretboardPos::maxString, GuitarFretboardPos::maxFret});
}

MainWindow::~MainWindow() = default;

void MainWindow::start()
{
    m_dialog.reset(new ConnectionDialog);
    connect(m_dialog.data(), &ConnectionDialog::connectAnyRequested, this, &MainWindow::connectAnyRequested);
    connect(m_dialog.data(), &ConnectionDialog::connectByAddressRequested, this, &MainWindow::connectByAddressRequested);
    m_dialog->exec();

    show();
}


void MainWindow::connected()
{
    if(m_dialog) m_dialog->connected();
}

void MainWindow::connectionFailed()
{
    if(m_dialog) m_dialog->connectionFailed();
}

void MainWindow::initEdgeWidgets()
{
    using Col = CubeEdge::Color;
    m_color2edges.clear();

    m_color2edges[Col::GREEN] = m_ui->greenEdgeWidget;
    m_color2edges[Col::BLUE] = m_ui->blueEdgeWidget;
    m_color2edges[Col::RED] = m_ui->redEdgeWidget;
    m_color2edges[Col::ORANGE] = m_ui->orangeEdgeWidget;
    m_color2edges[Col::WHITE] = m_ui->whiteEdgeWidget;
    m_color2edges[Col::YELLOW] = m_ui->yellowEdgeWidget;

    for (auto key: m_color2edges.keys())
    {
        auto w = m_color2edges.value(key);
        w->setEdgeButtonColor(key);

        // Set minimum by default
        const auto minString = GuitarFretboardPos().string;
        w->setMinStringNumber(minString);
    }
}

QList<EdgeSettingsWidget *> MainWindow::edgeWidgets()
{
    assert(!m_color2edges.empty());
    return m_color2edges.values();
}

void MainWindow::groupStringModeToggled(bool st)
{
    auto setStringForAllWidget = [this](int string)
    {
        for(auto w: edgeWidgets())
            w->setCurrentString(string);
    };

    auto bindEdgeWidgets = [this, setStringForAllWidget]
    {
        setStringForAllWidget(GuitarFretboardPos().string);
        for(auto w: edgeWidgets())
            connect(w, &EdgeSettingsWidget::stringChanged, this, setStringForAllWidget);
    };

    auto unleashEdgeWidgets = [this]
    {
        for(auto w: edgeWidgets())
            disconnect(w, 0, this, 0);
    };

    if(st)
        bindEdgeWidgets();
    else
        unleashEdgeWidgets();
}

void MainWindow::bedirectModeToggled(bool st)
{
    for(auto w: edgeWidgets())
        w->setRotationModeEnabled(st);
}

void MainWindow::setMaxGuitarFretboardPos(const GuitarFretboardPos & freatboardPos)
{
    for(auto w: edgeWidgets())
    {
        w->setMaxFretNumber(freatboardPos.fret);
        w->setMaxStringNumber(freatboardPos.string);
    }
}

GuitarFretboardPos MainWindow::guitarFretboardPosFor(const CubeEdge& info) const
{
    auto widget = m_color2edges.value(info.color, nullptr);
    if(!widget)
        return {};
    return {widget->stringFor(info.rotation), widget->fretFor(info.rotation)};
}

void MainWindow::highlightEdge(CubeEdge::Color color)
{
    if(auto edge = m_color2edges.value(color, nullptr))
        edge->blinkEdgeButton();
}

int MainWindow::soundDuration() const
{
    return m_ui->durLineEdit->text().toInt();
}
