#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <src/CubeEdge.h>
#include <src/Music.h>

namespace Ui {
class NotificationWidget;
}

class QTimer;

class NotificationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationWidget(QWidget *parent = 0);
    ~NotificationWidget();

    void notify(const CubeEdge& ce, const Music::Harmony&);
    void setIdleMessage(const QString&);

private:
    void setPage(QWidget *);

private:
    static const int m_notifyTime = 2000;

    QTimer * m_timer;
    QScopedPointer<Ui::NotificationWidget> m_ui;
};
