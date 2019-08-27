#include "SampleGenerator.h"

#include <QSound>
#include <QTimer>

SampleGenerator::SampleGenerator(QObject* parent):
    SoundGenerator(parent)
{
}

void SampleGenerator::playHarmony(const Music::Harmony & harm)
{
    qDeleteAll(m_players);
    m_players.clear();

    for(int i = 0; i != harm.tones.size(); ++i)
    {
        auto path = resourceFor(harm.tones[i]);
        auto player = new QSound(path, this);

        QTimer::singleShot(harm.delayMSec * i, player, [player] { player->play(); });

        m_players.append(player);
    }
}

QString SampleGenerator::resourceFor(const Music::Tone &) const
{
    return ":/sounds/beep.wav";
}

