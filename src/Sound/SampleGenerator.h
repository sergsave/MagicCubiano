#pragma once

#include "SoundGenerator.h"
#include <functional>

class QSoundEffect;

class SampleGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit SampleGenerator(const Music::Interval&, QObject* parent = nullptr);

protected:
    void setResourcePathFunc(const std::function<QString(const Music::Tone&)>& func);

private:
    void doPlay(const Music::Harmony& harm, int vol) override;
    QString resourcePathFor(const Music::Tone&) const;

private:
    Music::Interval m_interval;
    std::function<QString(const Music::Tone&)> m_pathFunc;

    QList<QSoundEffect*> m_players;
};
