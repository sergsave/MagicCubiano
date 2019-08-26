#include "GuitarGenerator.h"

#include <QSound>
#include <QTimer>
#include <QFileInfo>
#include <QDebug>

namespace {

QString resourceFor(const Music::Tone& tone)
{
    auto copy = tone;
    copy.octave -= 2;

    QString prefix = ":/sounds/";
    QString postfix = ".wav";

    return prefix + copy.toString(Music::Tone::Format::IS) + postfix;
}

}

GuitarGenerator::GuitarGenerator(QObject* parent):
    SoundGenerator(parent)
{
    auto fillPlayers = [this](const Music::Tones& tones)
    {
        for(auto t : tones)
        {
            auto path = resourceFor(t);
            if(QFileInfo(path).exists())
                m_players[t.toString()] = new QSound(path, this);
        }
    };

    fillPlayers(Music::allTonesForOctave(2));
    fillPlayers(Music::allTonesForOctave(3));
    fillPlayers(Music::allTonesForOctave(4));
    fillPlayers(Music::allTonesForOctave(5));
}

void GuitarGenerator::playHarmony(const Music::Harmony & harm)
{
    if(harm.tones.empty())
        return;

    QVector<QSound*> players;

    for(auto t: harm.tones)
    {
        if(auto p = m_players.value(t.toString(), nullptr))
            players.append(p);
    }

    for (int i = 0; i != players.size(); ++i)
    {
        QTimer::singleShot(harm.delayMSec * i, players[i], [players,i] () {
            players[i]->stop();
            players[i]->play();
        });
    }
}

