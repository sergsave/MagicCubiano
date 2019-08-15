#include "EdgeSettingsWidget.h"
#include "ui_EdgeSettingsWidget.h"

#include <cassert>
#include <QColor>

using Rotation = CubeEdge::Rotation;

namespace {

bool isClockwize(Rotation rot)
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

QColor colorFor(CubeEdge::Color color)
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

}

EdgeSettingsWidget::EdgeSettingsWidget(QWidget * parent) :
    QWidget(parent),
    m_ui(new Ui::EdgeSettingsWidget)
{
    m_ui->setupUi(this);

    auto bind = [](QSlider const * slider, QLabel * label) {
        label->setText(QString("%1").arg(slider->value()));
    };

    auto updateCounters = [this, bind] () {
        bind(m_ui->clockFretSlider, m_ui->clockFretValLabel);
        bind(m_ui->clockStringSlider, m_ui->clockStringValLabel);
        bind(m_ui->antiStringSlider, m_ui->antiStringValLabel);
        bind(m_ui->antiFretSlider, m_ui->antiFretValLabel);
    };

    auto sliderValueChanged = [this, updateCounters] (QSlider const * slider, int val) {

        if(slider == currStringSlider())
            emit stringChanged(val);
        if(slider == currFretSlider())
            emit fretChanged(val);

        updateCounters();
    };

    auto initSlider = [this, sliderValueChanged] (QSlider * slider) {
        connect(slider, &QSlider::valueChanged, this, [slider, sliderValueChanged](int v) {
            sliderValueChanged(slider, v);
        });
    };

    forEachFretSlider(initSlider);
    forEachStringSlider(initSlider);

    setRotationModeEnabled(false);
    setRotationPage(Rotation::CLOCKWIZE);

    connect(m_ui->rotationButton, &QPushButton::toggled, this, [this] (bool st) {
        setRotationPage(st ? Rotation::ANTICLOCKWIZE : Rotation::CLOCKWIZE);
    });
}

EdgeSettingsWidget::~EdgeSettingsWidget() = default;

void EdgeSettingsWidget::updateButtonIcon()
{
    auto button = m_ui->rotationButton;

    button->setStyleSheet(QString("background-color: %1").arg(colorFor(m_color).name()));

    // TODO: correct size on Android
    const QString path = isClockwize(m_rotationPage) ? ":/clockwize.png" : ":/anticlockwize.png";
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
        {m_ui->clockFretSlider,  m_ui->antiFretSlider},
        {m_ui->antiFretSlider,  m_ui->clockFretSlider},
        {m_ui->clockStringSlider,  m_ui->antiStringSlider},
        {m_ui->antiStringSlider,  m_ui->clockStringSlider},
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
        forEachStringSlider([this] (auto s) {
            s->setValue(this->currStringSlider()->value());
        });
        forEachFretSlider([this] (auto s) {
            s->setValue(this->currFretSlider()->value());
        });
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
        m_ui->stackedWidget->setCurrentWidget(m_ui->clockwizePage);
    else
        m_ui->stackedWidget->setCurrentWidget(m_ui->antiClockwizePage);

    m_rotationPage = page;

    updateButtonIcon();
}

void EdgeSettingsWidget::setMaxStringNumber(int n)
{
    forEachStringSlider([n] (QSlider * s) { s->setMaximum(n); });
}

void EdgeSettingsWidget::setMaxFretNumber(int n)
{
    forEachFretSlider([n] (QSlider * s) { s->setMaximum(n); });
}

void EdgeSettingsWidget::setMinStringNumber(int n)
{
    forEachStringSlider([n] (QSlider * s) { s->setMinimum(n); });
}

void EdgeSettingsWidget::setMinFretNumber(int n)
{
    forEachFretSlider([n] (QSlider * s) { s->setMinimum(n); });
}

void EdgeSettingsWidget::setCurrentString(int n)
{
    currStringSlider()->setValue(n);
}

void EdgeSettingsWidget::setCurrentFret(int n)
{
    currFretSlider()->setValue(n);
}

int EdgeSettingsWidget::stringFor(CubeEdge::Rotation rot) const
{
    if(isClockwize(rot))
        return m_ui->clockStringSlider->value();

    return m_ui->antiStringSlider->value();
}

int EdgeSettingsWidget::fretFor(CubeEdge::Rotation rot) const
{
    if(isClockwize(rot))
        return m_ui->clockFretSlider->value();

    return m_ui->antiFretSlider->value();
}

QSlider *EdgeSettingsWidget::currFretSlider() const
{
    if(isClockwize(m_rotationPage))
        return m_ui->clockFretSlider;

    return m_ui->antiFretSlider;
}

QSlider *EdgeSettingsWidget::currStringSlider() const
{
    if(isClockwize(m_rotationPage))
        return m_ui->clockStringSlider;

    return m_ui->antiStringSlider;
}

void EdgeSettingsWidget::forEachFretSlider(std::function<void (QSlider *)> func)
{
    for (auto s: { m_ui->clockFretSlider, m_ui->antiFretSlider }) func(s);
}

void EdgeSettingsWidget::forEachStringSlider(std::function<void (QSlider *)> func)
{
    for (auto s: { m_ui->clockStringSlider, m_ui->antiStringSlider }) func(s);
}

