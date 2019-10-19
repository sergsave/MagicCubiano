#include "Storage.h"

#include "Model.h"

namespace Preset {

Storage::~Storage() = default;

bool Storage::addPreset(const QString& name, AbstractPreset * preset)
{
    if(contains(name))
        return false;

    m_presets[name] = std::move(std::unique_ptr<AbstractPreset>(preset));
}

AbstractPreset * Storage::findPreset(const QString &name) const
{
    if(!contains(name))
        return nullptr;

    return m_presets.at(name).get();
}

bool Storage::renamePreset(const QString &oldName, const QString &newName)
{
    if(!contains(oldName) || contains(newName))
        return false;

    auto preset = std::move(m_presets[oldName]);

    m_presets.erase(oldName);
    m_presets[newName] = std::move(preset);

    return true;
}

bool Storage::removePreset(const QString &name)
{
    if(!contains(name))
        return false;

    m_presets.erase(name);
    return true;
}

QStringList Storage::allPresets() const
{
    QStringList ret;
    for(auto& kv: m_presets)
        ret.append(kv.first);
    return ret;
}

bool Storage::contains(const QString &name) const
{
    return m_presets.count(name) != 0;
}

QString generateVacantName(const Storage &storage, const QString &sourceName)
{
    QString name = sourceName;
    int i = 1;

    while(storage.findPreset(name))
        name = QString("%1 (%2)").arg(sourceName).arg(i++);

    return name;
}

}
