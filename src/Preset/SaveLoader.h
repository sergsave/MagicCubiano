#pragma once

#include <QScopedPointer>
#include "Model.h"

namespace Preset {

class Storage;

class SaveLoader
{
public:
    SaveLoader(Storage * storage);
    ~SaveLoader();

// TODO: check result
    bool load(const QString& dir, const QString& name);
    bool save(const QString& dir, const QString& name);

private:
    Storage * m_storage = nullptr;
};

}
