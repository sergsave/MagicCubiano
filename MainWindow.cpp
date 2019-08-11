#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QVariant>

#include <cassert>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set minimum by default
    auto minString = GuitarFretboardPos().string;
    auto widgets = findChildren<EdgeSettingsWidget*>();
    for(auto w: widgets) w->setMinStringNumber(minString);

    connect(ui->bidrectCheckBox, &QCheckBox::toggled, this, &MainWindow::bedirectModeToggled);
    connect(ui->groupStringCheckBox, &QCheckBox::toggled, this, &MainWindow::groupStringModeToggled);

    setMaxGuitarFretboardPos({GuitarFretboardPos::maxString, GuitarFretboardPos::maxFret});
    setEdgeWidgetsColor();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setEdgeWidgetsColor()
{
    using Col = CubeEdge::Color;
    m_color2edges.clear();

    m_color2edges[Col::GREEN] = ui->greenEdgeWidget;
    m_color2edges[Col::BLUE] = ui->blueEdgeWidget;
    m_color2edges[Col::RED] = ui->redEdgeWidget;
    m_color2edges[Col::ORANGE] = ui->orangeEdgeWidget;
    m_color2edges[Col::WHITE] = ui->whiteEdgeWidget;
    m_color2edges[Col::YELLOW] = ui->yellowEdgeWidget;

    for (auto key: m_color2edges.keys())
        m_color2edges.value(key)->setEdgeColor(key);
}

void MainWindow::groupStringModeToggled(bool st)
{
    auto widgets = findChildren<EdgeSettingsWidget*>();

    auto setStringForAllWidget = [widgets](int string)
    {
        for(auto w: widgets)
            w->setCurrentString(string);
    };

    auto bindEdgeWidgets = [this, widgets, setStringForAllWidget]
    {
        setStringForAllWidget(GuitarFretboardPos().string);
        for(auto w: widgets)
            connect(w, &EdgeSettingsWidget::stringChanged, this, setStringForAllWidget);
    };

    auto unleashEdgeWidgets = [this, widgets]
    {
        for(auto w: widgets)
            disconnect(w, 0, this, 0);
    };

    if(st)
        bindEdgeWidgets();
    else
        unleashEdgeWidgets();
}

void MainWindow::bedirectModeToggled(bool st)
{
    auto widgets = findChildren<EdgeSettingsWidget*>();
    for(auto w: widgets)
        w->setRotationModeEnabled(st);
}

void MainWindow::setMaxGuitarFretboardPos(const GuitarFretboardPos & freatboardPos)
{
    auto widgets = findChildren<EdgeSettingsWidget*>();
    for(auto w: widgets)
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

int MainWindow::soundDuration() const
{
    return ui->durLineEdit->text().toInt();
}

void MainWindow::setStatus(const QString& text)
{
    ui->statusTextLabel->setText(text);
}
