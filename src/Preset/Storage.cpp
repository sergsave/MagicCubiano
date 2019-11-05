#include "Storage.h"

#include "Presets.h"

namespace Preset {

Storage::~Storage() = default;

bool Storage::addPreset(const QString& name, AbstractPreset * preset)
{
    if(contains(name))
        return false;

    m_names.append(name);
    m_name2preset[name] = std::move(std::unique_ptr<AbstractPreset>(preset));
    return true;
}

AbstractPreset * Storage::findPreset(const QString &name) const
{
    if(!contains(name))
        return nullptr;

    return m_name2preset.at(name).get();
}

bool Storage::renamePreset(const QString &oldName, const QString &newName)
{
    if(!contains(oldName) || contains(newName))
        return false;

    auto idx = m_names.indexOf(oldName);
    m_names[idx] = newName;

    auto preset = std::move(m_name2preset[oldName]);

    m_name2preset.erase(oldName);
    m_name2preset[newName] = std::move(preset);

    return true;
}

bool Storage::removePreset(const QString &name)
{
    if(!contains(name))
        return false;

    m_name2preset.erase(name);
    m_names.removeOne(name);

    return true;
}

QStringList Storage::allPresetNames() const
{
    return m_names;
}

bool Storage::contains(const QString &name) const
{
    return m_names.contains(name);
}

}
