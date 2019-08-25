#pragma once

#include <QObject>

#include "src/Music.h"

class SoundGenerator : public QObject
{
public:
    using QObject::QObject;
    virtual ~SoundGenerator() = default;

    virtual void playHarmony(const Music::Harmony&) = 0;
};

