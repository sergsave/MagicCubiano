#include "SampleGenerator.h"

#include <QSound>
#include <QTimer>

SampleGenerator::SampleGenerator(QObject* parent):
    SoundGenerator(parent)
{
}

void SampleGenerator::playHarmony(const Music::Harmony & harm)
{
    for(int i = 0; i != harm.tones.size(); ++i)
    {
        auto path = resourceFor(harm.tones[i]);
        QTimer::singleShot(harm.delayMSec * i, this, [path] { QSound::play(path); });
    }
}

QString SampleGenerator::resourceFor(const Music::Tone &) const
{
    return ":/sounds/beep.wav";
}

