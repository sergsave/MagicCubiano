#pragma once

#include "SoundGenerator.h"

#include <QObject>
#include <QMap>

class QSound;

class GuitarGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit GuitarGenerator(QObject* parent = nullptr);

    void playHarmony(const Music::Harmony& ) override;

private:
    QMap<QString, QSound*> m_players;
};
