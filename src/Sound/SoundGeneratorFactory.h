#pragma once

#include <cassert>

#include "ToneGenerator.h"
/*#include "GuitarGenerator.h"*/

enum class GenType
{
    TONE,
    GUITAR
};

SoundGenerator * createSoundGenerator(GenType type, QObject * parent = nullptr)
{
    switch (type)
    {
    case GenType::TONE:
        return new ToneGenerator(parent);
    case GenType::GUITAR:
        // return new GuitarGenerator(parent);
        return nullptr;
    default:
        assert("unreal logic");
        return nullptr;
    }
}

