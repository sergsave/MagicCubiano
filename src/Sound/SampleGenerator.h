#pragma once

#include "SoundGenerator.h"

class QSound;

class SampleGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit SampleGenerator(QObject* parent = nullptr);

    void playHarmony(const Music::Harmony& ) override;
    virtual QString resourceFor(const Music::Tone&) const;

private:
    QList<QSound*> m_players;
};
