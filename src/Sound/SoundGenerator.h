#pragma once

#include <QObject>

#include "src/Music.h"

class SoundGenerator : public QObject
{
public:
    SoundGenerator(QObject * parent = nullptr) : QObject(parent) {}
    virtual ~SoundGenerator() = default;

    void play(const Music::Harmony& harm, int volume = 100)
    {
        doPlay(harm, volume);
    }

private:
    // NVI Idiom
    virtual void doPlay(const Music::Harmony& harm, int volume) = 0;
};

