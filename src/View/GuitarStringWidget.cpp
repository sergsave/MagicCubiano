#include "GuitarStringWidget.h"
#include "ui_GuitarStringWidget.h"

GuitarStringWidget::GuitarStringWidget(int number, int minFret, int maxFret, QWidget *parent):
    QWidget(parent),
    m_ui(new Ui::GuitarStringWidget)
{
    m_ui->setupUi(this);

    m_ui->fretSlider->setMinimum(minFret);
    m_ui->fretSlider->setMaximum(maxFret);

    m_ui->stringNumberLabel->setText(QString::number(number));
    setMuted(true);

    connect(m_ui->fretSlider, &QSlider::valueChanged, this, [this](int val) {
        m_ui->muteCheckBox->setChecked(true);
        m_ui->fretNumberLabel->setText(QString::number(val));
        emit fretValueChanged(val);
    });

    connect(m_ui->muteCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit muteChaged(!checked);
    });
}

GuitarStringWidget::~GuitarStringWidget() = default;

int GuitarStringWidget::number() const
{
    return m_ui->stringNumberLabel->text().toInt();
}

bool GuitarStringWidget::isMuted() const
{
    return !m_ui->muteCheckBox->isChecked();
}

void GuitarStringWidget::setMuted(bool st)
{
    if(st != m_ui->muteCheckBox->isChecked())
        return;

    m_ui->muteCheckBox->setChecked(!st);
    emit muteChaged(st);
}

int GuitarStringWidget::fretValue() const
{
    return m_ui->fretSlider->value();
}

void GuitarStringWidget::setFretValue(int val)
{
    if(val == m_ui->fretSlider->value())
        return;

    m_ui->fretSlider->setValue(val);
    emit fretValueChanged(val);
}

void GuitarStringWidget::reset()
{
    m_ui->fretSlider->setValue(0);
    m_ui->muteCheckBox->setChecked(false);
}
