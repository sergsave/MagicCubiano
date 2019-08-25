#pragma once

#include "SoundGenerator.h"

#include <QObject>

class QMediaPlayer;

class GuitarGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit GuitarGenerator(QObject* parent = nullptr);

    void playHarmony(const Music::Harmony& ) override;

private:
    QVector<QMediaPlayer*> m_players;
};
