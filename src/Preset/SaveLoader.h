#pragma once

#include "Presets.h"

namespace Preset {

class AbstractPreset;

class SaveLoader
{
public:
    SaveLoader();
    ~SaveLoader();

    // load method create new preset, doesn't take ownership
    AbstractPreset * load(const QString& filePath);
    bool save(const QString& filePath, AbstractPreset const * preset);
};

}
