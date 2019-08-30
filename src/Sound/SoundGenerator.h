#pragma once

#include <QObject>

#include "src/Music.h"

class SoundGenerator : public QObject
{
public:
    SoundGenerator(QObject * parent = nullptr) : QObject(parent) {}
    virtual ~SoundGenerator() = default;

    virtual void playHarmony(const Music::Harmony&) = 0;
};

