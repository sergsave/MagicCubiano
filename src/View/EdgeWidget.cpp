#include "EdgeWidget.h"
#include "ui_EdgeWidget.h"

#include <cassert>

#include <QColor>
#include <QTimer>
#include <QSize>

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

EdgeWidget::EdgeWidget(CubeEdge::Color col, EdgeSettingsFactory * factory, QWidget * parent) :
    QWidget(parent),
    m_ui(new Ui::EdgeWidget)
{
    m_ui->setupUi(this);

    setEdgeColor(col);
    setRotationModeEnabled(false);
    setRotationPage(Rotation::CLOCKWIZE);

    connect(m_ui->rotationButton, &QAbstractButton::toggled, this, [this] (bool st) {
        setRotationPage(st ? Rotation::ANTICLOCKWIZE : Rotation::CLOCKWIZE);
    });

    connect(m_ui->clockwizeButton, &QAbstractButton::clicked, this, &EdgeWidget::enterSettings);
    connect(m_ui->antiClockwizeButton, &QAbstractButton::clicked, this, &EdgeWidget::enterSettings);

    m_rotation2settings[Rotation::CLOCKWIZE] = factory->create(this);
    m_rotation2settings[Rotation::ANTICLOCKWIZE] = factory->create(this);

    updateSettingsButtons();
}

EdgeWidget::~EdgeWidget() = default;

void EdgeWidget::indicate()
{
    const int blinkTime = 200;
    auto button = m_ui->rotationButton;

    // reset
    setWidgetStyleColor(button, "");

    QTimer::singleShot(blinkTime, button, [this, button] {
        setWidgetStyleColor(button, colorStrFor(m_color));
    });
}

void EdgeWidget::setEdgeColor(CubeEdge::Color color)
{
    setWidgetStyleColor(m_ui->rotationButton, colorStrFor(color));
    m_color = color;
}

CubeEdge::Color EdgeWidget::edgeColor() const
{
    return m_color;
}

void EdgeWidget::setRotationModeEnabled(bool enabled)
{
    m_rotationMode = enabled;
    updateRotationButton();
}

bool EdgeWidget::inRotationMode(bool) const
{
    return m_rotationMode;
}

void EdgeWidget::updateSettingsButtons()
{
    // TODO: Note icon
    auto text = [this](Rotation rot)
    {
        for(auto t: harmonyFor(rot).tones)
            text += t.toString() + " ";
        return text;
    };

    m_ui->clockwizeButton->setText(text(Rotation::CLOCKWIZE));
    m_ui->antiClockwizeButton->setText(text(Rotation::ANTICLOCKWIZE));
}

void EdgeWidget::enterSettings()
{
    m_rotation2settings[m_rotationPage]->exec();
    updateSettingsButtons();
}

void EdgeWidget::updateRotationButton()
{
    auto button = m_ui->rotationButton;

    if(!m_rotationMode)
    {
        button->setIcon({});
        button->setEnabled(false);
        return;
    }

    const QString path = isClockwize(m_rotationPage) ? ":/clockwize.png" : ":/anticlockwize.png";

    button->setIconSize(button->size() * 3 / 4);
    button->setIcon(QPixmap(path));
    button->setEnabled(true);
}

void EdgeWidget::setRotationPage(Rotation page)
{
    if(isClockwize(page))
        m_ui->stackedWidget->setCurrentWidget(m_ui->clockwizePage);
    else
        m_ui->stackedWidget->setCurrentWidget(m_ui->antiClockwizePage);

    m_rotationPage = page;

    updateRotationButton();
}

Music::Harmony EdgeWidget::harmonyFor(Rotation rot) const
{
    if(!m_rotationMode)
        return currentHarmony();

    return m_rotation2settings[rot]->harmony();
}

Music::Harmony EdgeWidget::currentHarmony() const
{
    return m_rotation2settings[m_rotationPage]->harmony();
}
