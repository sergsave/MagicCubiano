#include "GuitarGenerator.h"

#include <QMediaPlayer>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QTime>

namespace {

QUrl resourceFor(const Music::Tone& tone)
{
    auto copy = tone;
    copy.octave -= 2;

    QString prefix = "qrc:/sounds/";
    QString postfix = ".mp3";

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
            auto player = new QMediaPlayer(this);
            player->setMedia(resourceFor(t));
            m_players[t.toString()] = player;
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

    QVector<QMediaPlayer*> players;

    for(auto t: harm.tones)
        players.append(m_players[t.toString()]);

    qDebug() << QTime::currentTime();

    for (int i = 0; i != players.size(); ++i)
        QTimer::singleShot(harm.delayMSec * i, players[i], [players,i] ()
        {
            qDebug() << QTime::currentTime();

            players[i]->stop();
            players[i]->play();
        }

    );
}

