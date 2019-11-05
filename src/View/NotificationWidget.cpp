#include "NotificationWidget.h"
#include "ui_NotificationWidget.h"

#include <QTimer>

#include "Utils.h"

NotificationWidget::NotificationWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::NotificationWidget),
    m_timer(new QTimer(this))
{
    m_ui->setupUi(this);
    setLabelPixmap(m_ui->noteIconLabel, ":/images/musical-note.png");
    setPage(m_ui->idlePage);

    connect(m_timer, &QTimer::timeout, this, [this] { setPage(m_ui->idlePage); });
}

NotificationWidget::~NotificationWidget() = default;

void NotificationWidget::notify(const CubeEdge &ce, const Music::Harmony& harmony)
{
    setPage(m_ui->notifyPage);

    auto appearance = appearanceFor(ce);
    setLabelPixmap(m_ui->rotationIconLabel, appearance.iconPath);
    m_ui->notifyFrame->setStyleSheet(QString("background-color: %1").arg(appearance.color.name()));

    auto tones = harmony.tones;
    QString text = tones.isEmpty() ? "No tones" : tonesToString(tones);
    m_ui->notesLabel->setText(text);

    m_timer->start(m_notifyTime);
}

void NotificationWidget::setIdleMessage(const QString& msg)
{
    m_ui->idleMessageLabel->setText(msg);
}

void NotificationWidget::setPage(QWidget * w)
{
    m_ui->stackedWidget->setCurrentWidget(w);
}

