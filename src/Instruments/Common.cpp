#include "Common.h"

namespace Instruments {

Type typeFrom(const GuitarTag &)         { return Type::GUITAR; }
Type typeFrom(const ElectricGuitarTag &) { return Type::ELECTRIC_GUITAR; }
Type typeFrom(const PianoTag &)          { return Type::PIANO; }

}

