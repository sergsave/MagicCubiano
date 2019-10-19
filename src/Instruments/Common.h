#pragma once

#include "Descriptions.h"

namespace Instruments {

enum class Type
{
    GUITAR,
    ELECTRIC_GUITAR,
    PIANO
};

static Type typeFrom(const GuitarTag& )         { return Type::GUITAR; }
static Type typeFrom(const ElectricGuitarTag& ) { return Type::ELECTRIC_GUITAR; }
static Type typeFrom(const PianoTag& )          { return Type::PIANO; }

}
