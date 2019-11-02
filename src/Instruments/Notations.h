#pragma once

#include "src/Music.h"

namespace Instruments {

struct GuitarNotation
{
    int string;
    int fret;

    GuitarNotation(int string = 0, int fret = 0): string(string), fret(fret) {}
};

bool operator == (const GuitarNotation& lhs, const GuitarNotation& rhs);
bool operator > (const GuitarNotation& lhs, const GuitarNotation& rhs);
bool operator < (const GuitarNotation& lhs, const GuitarNotation& rhs);

using PianoNotation = Music::Tone;

}
