#pragma once

#include "SoundGenerator.h"
#include <functional>

class QSoundEffect;

class SampleGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    using SoundGenerator::SoundGenerator;

    Music::Interval interval() const;

protected:
    using PathFunc = std::function<QString(const Music::Tone&)>;

    SampleGenerator(const PathFunc&, const Music::Interval&, QObject * parent);

private:
    void doPlay(const Music::Harmony& harm, int vol) override;
    QString resourcePathFor(const Music::Tone&) const;

private:
    Music::Interval m_interval;
    PathFunc m_pathFunc;

    QList<QSoundEffect*> m_players;
};
