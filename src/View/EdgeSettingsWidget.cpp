#include "EdgeSettingsWidget.h"
#include "ui_EdgeSettingsWidget.h"

#include <cassert>

#include <QColor>
#include <QTimer>
#include <QSize>

using Rotation = CubeEdge::Rotation;

namespace {

const int g_buttonWidth { 140 };

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

QString colorStrFor(CubeEdge::Color color)
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

    return map.value(color, Qt::black).name();
}

void setWidgetStyleColor(QWidget * w, const QString& color)
{
    if(color.isEmpty())
    {
        w->setStyleSheet({});
        return;
    }

    const QString style = QString("background-color: %1").arg(color);
    w->setStyleSheet(style);
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

    auto button = m_ui->rotationButton;
    connect(button, &QAbstractButton::toggled, this, [this] (bool st) {
        setRotationPage(st ? Rotation::ANTICLOCKWIZE : Rotation::CLOCKWIZE);
    });

    button->setFixedWidth(g_buttonWidth);
}

EdgeSettingsWidget::~EdgeSettingsWidget() = default;

void EdgeSettingsWidget::blinkEdgeButton()
{
    const int blinkTime = 200;
    auto button = m_ui->rotationButton;

    // reset
    setWidgetStyleColor(button, "");

    QTimer::singleShot(blinkTime, button, [this, button] {
        setWidgetStyleColor(button, colorStrFor(m_color));
    });
}

void EdgeSettingsWidget::setEdgeButtonColor(CubeEdge::Color color)
{
    m_color = color;
    setWidgetStyleColor(m_ui->rotationButton, colorStrFor(m_color));
}

CubeEdge::Color EdgeSettingsWidget::edgeButtonColor() const
{
    return m_color;
}

void EdgeSettingsWidget::updateButtonIcon()
{
    auto button = m_ui->rotationButton;

    if(!m_rotationMode)
    {
        button->setIcon({});
        return;
    }

    const QString path = isClockwize(m_rotationPage) ? ":/clockwize.png" : ":/anticlockwize.png";

    button->setIconSize(button->size() * 3 / 4);
    button->setIcon(QPixmap(path));
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
    updateButtonIcon();
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

bool EdgeSettingsWidget::inRotationMode(bool) const
{
    return m_rotationMode;
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

