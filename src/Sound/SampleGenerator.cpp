#include "SampleGenerator.h"

#include <cassert>

#include <QSoundEffect>
#include <QAudio>
#include <QTimer>
#include <QFileInfo>

SampleGenerator::SampleGenerator(const PathFunc& func, const Music::Interval& interval, QObject * parent):
    SoundGenerator(parent),
    m_interval(interval),
    m_pathFunc(func)
{
    // Check of all resources are presented
    for(auto& tone: Music::allTonesFor(m_interval))
    {
        QFileInfo info(func(tone));
        if(!info.exists())
        {
            assert(!"resource not found");
            break;
        }
    }

    m_pathFunc = func;
}

Music::Interval SampleGenerator::interval() const
{
    return m_interval;
}

void SampleGenerator::doPlay(const Music::Harmony & harm, int volume)
{
    for(auto p: m_players)
    {
        p->setMuted(true);

        const int antiClickDelay = 10;
        QTimer::singleShot(antiClickDelay, p, &QObject::deleteLater);
    }
    m_players.clear();

    for(int i = 0; i != harm.tones.size(); ++i)
    {
        auto path = resourcePathFor(harm.tones[i]);
        auto player = new QSoundEffect(this);
        player->setSource(QUrl::fromLocalFile(path));

        qreal linearVolume = QAudio::convertVolume(volume / qreal(100.0),
                                                   QAudio::LogarithmicVolumeScale,
                                                   QAudio::LinearVolumeScale);
        player->setVolume(linearVolume);

        QTimer::singleShot(harm.minToneDurationMSec * i, player, [player] { player->play(); });

        m_players.append(player);
    }
}

QString SampleGenerator::resourcePathFor(const Music::Tone & tone) const
{
    QString beepPath = ":/sounds/beep.wav";

    if(!m_pathFunc)
        return beepPath;

    auto path = m_pathFunc(tone);
    QFileInfo info(path);

    if(info.exists())
        return path;

    return beepPath;
}
