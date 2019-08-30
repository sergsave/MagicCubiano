#pragma once

#include "SoundGenerator.h"
#include <functional>

class QSound;

class SampleGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit SampleGenerator(const Music::Interval&, QObject* parent = nullptr);

    void playHarmony(const Music::Harmony& ) override;

protected:
    void setResourcePathFunc(const std::function<QString(const Music::Tone&)>& func);

private:
    QString resourcePathFor(const Music::Tone&) const;

private:
    Music::Interval m_interval;
    std::function<QString(const Music::Tone&)> m_pathFunc;

    QList<QSound*> m_players;
};
