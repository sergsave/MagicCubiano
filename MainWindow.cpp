#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QVariant>

#include <cassert>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto widgets = findChildren<EdgeSettingsWidget*>();
    for(auto w: widgets)
        w->setName(w->property("name").toString());

    ui->allStringsSlider->setMinimum(MusicInfo().stringNumber);

    connect(ui->allStringsSlider, &QSlider::valueChanged, ui->allStringsLabel,
            [this](auto val) { ui->allStringsLabel->setText(QString("%1").arg(val));});

    connect(ui->resetStringsButton, &QPushButton::clicked, this,
            [widgets, this] { for (auto w: widgets) w->setCurrentString(ui->allStringsSlider->value());});

    setMaximumMusicInfo({});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMaximumMusicInfo(const MusicInfo & info)
{
    auto widgets = findChildren<EdgeSettingsWidget*>();
    for(auto w: widgets)
    {
        w->setMaxFretNumber(info.fretNumber);
        w->setMaxStringNumber(info.stringNumber);
    }

    ui->allStringsSlider->setMaximum(info.stringNumber);
}

MainWindow::MusicInfo MainWindow::musicInfoFor(EdgeType type) const
{
    auto musicInfo = [] (EdgeSettingsWidget const * widget)
    {
        return MusicInfo(widget->currentString(), widget->currentFret());
    };

    using E = EdgeType;
    switch (type) {
    case E::UPPER:
        return musicInfo(ui->uEdgeWidget);
    case E::DOWN:
        return musicInfo(ui->dEgdeWidget);
    case E::FRONT:
        return musicInfo(ui->fEdgeWidget);
    case E::BACK:
        return musicInfo(ui->bEdgeWidget);
    case E::RIGHT:
        return musicInfo(ui->rEdgeWidget);
    case E::LEFT:
        return musicInfo(ui->lEdgeWidget);
    default:
        assert(!"Unknown type");
        return MusicInfo();
    }
}

int MainWindow::soundDuration() const
{
    return ui->durLineEdit->text().toInt();
}

void MainWindow::setStatus(const QString& text)
{
    ui->statusTextLabel->setText(text);
}
