#include "FactoryMethod.h"
#include "Presets.h"

namespace Preset {

class AbstractPreset;

AbstractPreset * createPreset(Instruments::Type type)
{
    using T = Instruments::Type;
    switch (type)
    {
    case T::GUITAR:
        return new GuitarPreset();
    case T::ELECTRIC_GUITAR:
        return new ElectricGuitarPreset();
    case T::PIANO:
        return new PianoPreset();
    default:
        return nullptr;
    }
}

}
