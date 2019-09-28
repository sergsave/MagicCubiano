#pragma once

#include <cassert>

#include "ToneGenerator.h"
#include "GuitarGenerators.h"

#include "src/Configuration.h"

SoundGenerator * createSoundGenerator(Music::Instrument type, QObject * parent = nullptr)
{    
    auto interval = Configuration::intervalFor(type);

    switch (type)
    {
    case Music::Instrument::BEEPER:
        return new ToneGenerator(interval, parent);
    case Music::Instrument::GUITAR:
        return new GuitarGenerator(interval, parent);
    case Music::Instrument::DISTORTION_GUITAR:
        return new DistortionGuitarGenerator(interval, parent);
    default:
        assert(!"unsupported");
        return nullptr;
    }
}

