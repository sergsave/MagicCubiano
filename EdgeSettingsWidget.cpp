#include "EdgeSettingsWidget.h"
#include "ui_EdgeSettingsWidget.h"

#include <cassert>
#include <QColor>

using Rotation = CubeEdge::Rotation;

static bool isClockwize(Rotation rot)
{
    switch (rot)
    {
    case Rotation::CLOCKWIZE:
        return true;
    case Rotation::ANTICLOCKWIZE:
        return false;
    default:
        assert(!"Unknow direction!");
        return false;
    }
}

static QColor colorFor(CubeEdge::Color color)
{
    using Col = CubeEdge::Color;
    QMap<Col, QColor> map
    {
        { Col::GREEN, Qt::darkGreen },
        { Col::BLUE, Qt::darkBlue },
        { Col::RED, "#FFB6C1" }, // Light pink Giiker V1 peculiarity
        { Col::ORANGE, "#FF4500"},  // Orangered Giiker V1 peculiarity
        { Col::YELLOW, Qt::yellow},
        { Col::WHITE, Qt::white}
    };

    return map.value(color, Qt::black);
}

EdgeSettingsWidget::EdgeSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EdgeSettingsWidget)
{
    ui->setupUi(this);

    auto bind = [](QSlider const * slider, QLabel * label) {
        label->setText(QString("%1").arg(slider->value()));
    };

    auto updateCounters = [this, bind] () {
        bind(ui->clockFretSlider, ui->clockFretValLabel);
        bind(ui->clockStringSlider, ui->clockStringValLabel);
        bind(ui->antiStringSlider, ui->antiStringValLabel);
        bind(ui->antiFretSlider, ui->antiFretValLabel);
    };

    auto sliderValueChanged = [this, updateCounters] (QSlider * slider, int val) {

        if(slider == stringSlider())
            emit stringChanged(val);
        if(slider == fretSlider())
            emit fretChanged(val);

        updateCounters();
    };

    auto initSlider = [this, sliderValueChanged] (QSlider * slider) {
        connect(slider, &QSlider::valueChanged, this, [slider, sliderValueChanged](int v) {
            sliderValueChanged(slider, v);
        });
    };

    initSlider(ui->clockStringSlider);
    initSlider(ui->antiStringSlider);
    initSlider(ui->clockFretSlider);
    initSlider(ui->antiFretSlider);

    setRotationModeEnabled(false);
    setRotationPage(Rotation::CLOCKWIZE);

    connect(ui->rotationButton, &QPushButton::toggled, this, [this] (bool st) {
        setRotationPage(st ? Rotation::ANTICLOCKWIZE : Rotation::CLOCKWIZE);
    });
}

EdgeSettingsWidget::~EdgeSettingsWidget()
{
    delete ui;
}

void EdgeSettingsWidget::updateButtonIcon()
{
    auto button = ui->rotationButton;

    button->setStyleSheet(QString("background-color: %1").arg(colorFor(m_color).name()));

    const QString path = isClockwize(m_rotationPage) ? ":/images/clockwize.png" : ":/images/anticlockwize.png";
    const QSize size {40, 40};

    button->setIconSize(size);
    button->setIcon(QIcon(path));
}

void EdgeSettingsWidget::setEdgeColor(CubeEdge::Color color)
{
    m_color = color;
    updateButtonIcon();
}

CubeEdge::Color EdgeSettingsWidget::edgeColor() const
{
    return m_color;
}

void EdgeSettingsWidget::setRotationModeEnabled(bool enabled)
{
    const QMap<QSlider *, QSlider *> sliders
    {
        {ui->clockFretSlider,  ui->antiFretSlider},
        {ui->antiFretSlider,  ui->clockFretSlider},
        {ui->clockStringSlider,  ui->antiStringSlider},
        {ui->antiStringSlider,  ui->clockStringSlider},
    };

    auto bindSliders = [] (QSlider * s1, QSlider * s2) {
        connect(s1, &QSlider::valueChanged, s2, &QSlider::setValue);
    };

    auto unleashSliders = [this](QSlider * s1, QSlider * s2) {
        disconnect(s1, 0, s2, 0);
    };

    for(auto key : sliders.keys())
    {
        auto value = sliders.value(key);

        if(enabled)
            unleashSliders(key, value);
        else
            bindSliders(key, value);

    }

    auto syncSlidersValue = [this] {
        ui->clockStringSlider->setValue(stringSlider()->value());
        ui->antiStringSlider->setValue(stringSlider()->value());
        ui->clockFretSlider->setValue(fretSlider()->value());
        ui->antiFretSlider->setValue(fretSlider()->value());
    };

    if(!enabled)
        syncSlidersValue();

    m_rotationMode = enabled;
}

bool EdgeSettingsWidget::inRotationMode(bool) const
{
    return m_rotationMode;
}

void EdgeSettingsWidget::setRotationPage(Rotation page)
{
    if(isClockwize(page))
        ui->stackedWidget->setCurrentWidget(ui->clockwizePage);
    else
        ui->stackedWidget->setCurrentWidget(ui->antiClockwizePage);

    m_rotationPage = page;

    updateButtonIcon();
}

void EdgeSettingsWidget::setMaxStringNumber(int n)
{
    ui->clockStringSlider->setMaximum(n);
    ui->antiStringSlider->setMaximum(n);
}

void EdgeSettingsWidget::setMaxFretNumber(int n)
{
    ui->clockFretSlider->setMaximum(n);
    ui->antiFretSlider->setMaximum(n);
}

void EdgeSettingsWidget::setMinStringNumber(int n)
{
    ui->clockStringSlider->setMinimum(n);
    ui->antiStringSlider->setMinimum(n);
}

void EdgeSettingsWidget::setMinFretNumber(int n)
{
    ui->clockFretSlider->setMinimum(n);
    ui->antiFretSlider->setMinimum(n);
}

void EdgeSettingsWidget::setCurrentString(int n)
{
    stringSlider()->setValue(n);
}

void EdgeSettingsWidget::setCurrentFret(int n)
{
    fretSlider()->setValue(n);
}

int EdgeSettingsWidget::stringFor(CubeEdge::Rotation rot) const
{
    if(isClockwize(rot))
        return ui->clockStringSlider->value();

    return ui->antiStringSlider->value();
}

int EdgeSettingsWidget::fretFor(CubeEdge::Rotation rot) const
{
    if(isClockwize(rot))
        return ui->clockFretSlider->value();

    return ui->antiFretSlider->value();
}

QSlider *EdgeSettingsWidget::fretSlider() const
{
    if(isClockwize(m_rotationPage))
        return ui->clockFretSlider;

    return ui->antiFretSlider;
}

QSlider *EdgeSettingsWidget::stringSlider() const
{
    if(isClockwize(m_rotationPage))
        return ui->clockStringSlider;

    return ui->antiStringSlider;
}

