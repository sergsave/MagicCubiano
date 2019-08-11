#include "EdgeSettingsWidget.h"
#include "ui_EdgeSettingsWidget.h"

EdgeSettingsWidget::EdgeSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EdgeSettingsWidget)
{
    ui->setupUi(this);

    auto updateCounters = [this] () {
        ui->fretValLabel->setText(QString("%1").arg(ui->fretSlider->value()));
        ui->stringValLabel->setText(QString("%1").arg(ui->stringSlider->value()));
    };

    connect(ui->stringSlider, &QSlider::valueChanged, this, updateCounters);
    connect(ui->fretSlider, &QSlider::valueChanged, this, updateCounters);
}

EdgeSettingsWidget::~EdgeSettingsWidget()
{
    delete ui;
}

void EdgeSettingsWidget::setName(const QString &text)
{
    ui->nameLabel->setText(text);
}

void EdgeSettingsWidget::setName(const QPixmap &pixmap)
{
    auto scaled = pixmap.scaled(ui->nameLabel->size());
    ui->nameLabel->setPixmap(scaled);
}

void EdgeSettingsWidget::setMaxStringNumber(int n)
{
    ui->stringSlider->setMaximum(n);
}

void EdgeSettingsWidget::setMaxFretNumber(int n)
{
    ui->fretSlider->setMaximum(n);
}

void EdgeSettingsWidget::setMinStringNumber(int n)
{
    ui->stringSlider->setMinimum(n);
}

void EdgeSettingsWidget::setMinFretNumber(int n)
{
    ui->fretSlider->setMinimum(n);
}

int EdgeSettingsWidget::currentString() const
{
    return ui->stringSlider->value();
}

int EdgeSettingsWidget::currentFret() const
{
    return ui->fretSlider->value();
}

void EdgeSettingsWidget::setCurrentString(int n)
{
    ui->stringSlider->setValue(n);
}

void EdgeSettingsWidget::setCurrentFret(int n)
{
    ui->fretSlider->setValue(n);
}
