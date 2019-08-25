#include "GuitarGenerator.h"

#include <QMediaPlayer>
#include <QUrl>
#include <QTimer>
#include <QDebug>

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
}

void GuitarGenerator::playHarmony(const Music::Harmony & harm)
{
    if(harm.tones.empty())
        return;

    qDeleteAll(m_players);
    m_players.clear();

    for (auto t : harm.tones)
    {
        auto player = new QMediaPlayer(this);
        player->setMedia(resourceFor(t));
        m_players.append(player);
    }

    for (int i = 0; i != m_players.size(); ++i)
        QTimer::singleShot(harm.delayMSec * i, m_players[i], &QMediaPlayer::play);
}

