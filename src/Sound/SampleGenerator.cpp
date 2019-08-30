#include "SampleGenerator.h"

#include <cassert>

#include <QSound>
#include <QTimer>
#include <QFileInfo>

SampleGenerator::SampleGenerator(const Music::Interval& interval, QObject* parent):
    SoundGenerator(parent),
    m_interval(interval)
{
}

void SampleGenerator::playHarmony(const Music::Harmony & harm)
{
    qDeleteAll(m_players);
    m_players.clear();

    for(int i = 0; i != harm.tones.size(); ++i)
    {
        auto path = resourcePathFor(harm.tones[i]);
        auto player = new QSound(path, this);

        QTimer::singleShot(harm.delayMSec * i, player, [player] { player->play(); });

        m_players.append(player);
    }
}

void SampleGenerator::setResourcePathFunc(const std::function<QString (const Music::Tone &)> &func)
{
    // Check of all resources are presented
    for(auto& tone: allTonesFor(m_interval))
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
