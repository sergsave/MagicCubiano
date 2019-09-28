#pragma once

#include <cassert>
#include <QList>

#include "Music.h"

// TODO: set in .ini or .txt
class Configuration
{
public:
    Configuration() = delete;

    static QList<Music::Instrument> supportedInstruments()
    {
        return { Ins::GUITAR, Ins::DISTORTION_GUITAR };
    }

    static int noteDurationMSec()
    {
        return 0;
    }

    static Music::Interval intervalFor(Music::Instrument type)
    {
        assert(supportedInstruments().contains(type) && "Unsupported instrument");

        switch (type)
        {
        case Ins::GUITAR:
        case Ins::DISTORTION_GUITAR:
            return { T(T::E, 2), T(T::B, 5) };
        default:
            return { };
        }
    }

private:
    using Ins = Music::Instrument;
    using T = Music::Tone;
};
