#pragma once

#include "src/Music.h"

namespace Instruments {

struct GuitarNotation
{
    int string;
    int fret;

    GuitarNotation(int string = 0, int fret = 0): string(string), fret(fret) {}
};

static bool operator == (const GuitarNotation& lhs, const GuitarNotation& rhs)
{
    return lhs.string == rhs.string && lhs.fret == rhs.fret;
}

static bool operator > (const GuitarNotation& lhs, const GuitarNotation& rhs)
{
    if(lhs.string != rhs.string)
        return lhs.fret > rhs.fret;

    return lhs.fret > rhs.fret;
}

static bool operator < (const GuitarNotation& lhs, const GuitarNotation& rhs)
{
    if(lhs.string != rhs.string)
        return lhs.fret < rhs.fret;

    return lhs.fret < rhs.fret;
}

using PianoNotation = Music::Tone;

}
