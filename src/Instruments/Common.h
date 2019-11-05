#pragma once

#include "Descriptions.h"

namespace Instruments {

enum class Type
{
    GUITAR,
    ELECTRIC_GUITAR,
    PIANO
};

Type typeFrom(const GuitarTag& );
Type typeFrom(const ElectricGuitarTag& );
Type typeFrom(const PianoTag& );

}
