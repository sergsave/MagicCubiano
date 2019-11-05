#pragma once

#include <cassert>

#include "GuitarGenerators.h"
#include "src/Instruments/Common.h"

SoundGenerator * createSoundGenerator(Instruments::Type type, QObject * parent = nullptr)
{    
    switch (type)
    {
    case Instruments::Type::GUITAR:
        return new GuitarGenerator(parent);
    case Instruments::Type::ELECTRIC_GUITAR:
        return new ElectricGuitarGenerator(parent);
    default:
        assert(!"unsupported");
        return nullptr;
    }
}

