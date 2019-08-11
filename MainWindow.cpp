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

    ui->allStringsSlider->setMinimum(minString);
    for(auto w: widgets) w->setMinStringNumber(minString);

    connect(ui->allStringsSlider, &QSlider::valueChanged, ui->allStringsLabel,
            [this](auto val) { ui->allStringsLabel->setText(QString("%1").arg(val));});

    connect(ui->resetStringsButton, &QPushButton::clicked, this,
            [widgets, this] { for (auto w: widgets) w->setCurrentString(ui->allStringsSlider->value());});

    setMaxGuitarFretboardPos({GuitarFretboardPos::maxString, GuitarFretboardPos::maxFret});
    setEdgeWidgetsName();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setEdgeWidgetsName()
{
    auto setPixmap = [this] (EdgeType type, const QColor& color)
    {
        auto w = edgeWidget(type);
        QPixmap pm(1, 1);
        pm.fill(color);

        if(w) w->setName(pm);
    };

    using E = EdgeType;

    setPixmap(E::GREEN, Qt::darkGreen);
    setPixmap(E::BLUE, Qt::darkBlue);
    setPixmap(E::RED, "#FFB6C1"); // Light pink Giiker V1 peculiarity
    setPixmap(E::ORANGE, "#FF4500");  // Orangered Giiker V1 peculiarity
    setPixmap(E::YELLOW, Qt::yellow);
    setPixmap(E::WHITE, Qt::white);
}

EdgeSettingsWidget * MainWindow::edgeWidget(EdgeType type) const
{
    using E = EdgeType;
    switch (type) {
    case E::GREEN:
        return ui->greenEdgeWidget;
    case E::BLUE:
        return ui->blueEdgeWidget;
    case E::RED:
        return ui->redEdgeWidget;
    case E::ORANGE:
        return ui->orangeEdgeWidget;
    case E::YELLOW:
        return ui->yellowEdgeWidget;
    case E::WHITE:
        return ui->whiteEdgeWidget;
    default:
        assert(!"Unknown type");
        return nullptr;
    }
}

void MainWindow::setMaxGuitarFretboardPos(const GuitarFretboardPos & freatboardPos)
{
    auto widgets = findChildren<EdgeSettingsWidget*>();
    for(auto w: widgets)
    {
        w->setMaxFretNumber(freatboardPos.fret);
        w->setMaxStringNumber(freatboardPos.string);
    }

    ui->allStringsSlider->setMaximum(freatboardPos.string);
}

GuitarFretboardPos MainWindow::guitarFretboardPos(EdgeType type) const
{
    auto widget = edgeWidget(type);
    if(!widget)
        return {};
    return {widget->currentString(), widget->currentFret()};
}

int MainWindow::soundDuration() const
{
    return ui->durLineEdit->text().toInt();
}

void MainWindow::setStatus(const QString& text)
{
    ui->statusTextLabel->setText(text);
}
