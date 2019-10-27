#pragma once

#include <QObject>

class SettingsModel: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

signals:
    void changed();

public slots:
    // Range 0..100
    void setVolume(int vol);

public:
    int volume() const;

private:
    int m_volume = 100;
};
