#pragma once

#include <map>
#include <memory>
#include <QStringList>

namespace Preset {

class AbstractPreset;

// Contains list of presets with unique names
class Storage
{
public:
    ~Storage();

    bool addPreset(const QString& name, AbstractPreset * preset);

    bool removePreset(const QString& name);
    bool renamePreset(const QString& oldName, const QString& newName);

    AbstractPreset * findPreset(const QString& name) const;
    QStringList allPresetNames() const;

private:
    bool contains(const QString& name) const;

private:
    QStringList m_names;
    std::map<QString, std::unique_ptr<AbstractPreset>> m_name2preset;
};

}


