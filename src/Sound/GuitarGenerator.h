#pragma once

#include "SoundGenerator.h"

#include <QObject>
#include <QMap>

class QMediaPlayer;

class GuitarGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit GuitarGenerator(QObject* parent = nullptr);

    void playHarmony(const Music::Harmony& ) override;

private:
    QMap<QString, QMediaPlayer*> m_players;
};
