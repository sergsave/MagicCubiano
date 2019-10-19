#pragma once

#include <map>
#include <memory>
#include <QString>

namespace Preset {

class AbstractPreset;

// Contains presets with unique names
class Storage
{
public:
    ~Storage();

    bool addPreset(const QString& name, AbstractPreset * preset);

    bool removePreset(const QString& name);
    bool renamePreset(const QString& oldName, const QString& newName);

    AbstractPreset * findPreset(const QString& name) const;
    QStringList allPresets() const;

private:
    bool contains(const QString& name) const;

private:
    std::map<QString, std::unique_ptr<AbstractPreset>> m_presets;
};

QString generateVacantName(const Storage& storage, const QString &sourceName);

}


