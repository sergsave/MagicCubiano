#pragma once

#include "src/Music.h"

namespace Instruments {

struct GuitarNotation
{
    int string;
    int fret;

    GuitarNotation(int string = 0, int fret = 0): string(string), fret(fret) {}
    // TODO: Operator >
};

using PianoNotation = Music::Tone;

}
