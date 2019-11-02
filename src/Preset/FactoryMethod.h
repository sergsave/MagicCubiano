#pragma once

#include "src/Instruments/Common.h"

namespace Preset {

class AbstractPreset;
AbstractPreset * createPreset(Instruments::Type type);

}


