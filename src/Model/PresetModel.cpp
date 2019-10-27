#include "Model.h"

#include <QFileInfo>
#include <QDir>

#include "src/Preset/Storage.h"
#include "src/Preset/SaveLoader.h"
#include "src/Preset/Presets.h"

using namespace Preset;

PresetModel::PresetModel(QObject *parent)
    : QObject(parent)
    , m_storage(new Storage())
    , m_saveLoader(new SaveLoader())
{}

PresetModel::~PresetModel() = default;

bool PresetModel::setActivePreset(const QString &name)
{
    if(!findPreset(name))
        return false;

    if(name == m_activePreset)
        return false;

    m_activePreset = name;
    emit changed();
    return true;
}

bool PresetModel::addPreset(const QString &name, AbstractPreset *preset)
{
    if(!m_storage->addPreset(name, preset))
        return false;

    if(!setActivePreset(name))
        emit changed();

    return true;
}

bool PresetModel::removePreset(const QString &name)
{
    auto indexOfRemoved = m_storage->allPresetNames().indexOf(name);
    if(!m_storage->removePreset(name))
        return false;

    auto indexOfActive = qBound(0, indexOfRemoved, m_storage->allPresetNames().size());
    auto active = m_storage->allPresetNames().value(indexOfActive);

    if(!setActivePreset(active))
        emit changed();

    return true;
}

bool PresetModel::renamePreset(const QString &oldName, const QString &newName)
{
    if(!m_storage->renamePreset(oldName, newName))
        return false;

    if(!setActivePreset(newName))
        emit changed();

    return true;
}

AbstractPreset *PresetModel::findPreset(const QString &name) const
{
    return m_storage->findPreset(name);
}

QStringList PresetModel::allPresets() const
{
    return m_storage->allPresetNames();
}

QString PresetModel::activePreset() const
{
    return m_activePreset;
}

bool PresetModel::savePresets(const QString &name, const QString &filePath)
{
    auto preset = findPreset(name);
    if(!preset)
        return false;

    return m_saveLoader->save(filePath, preset);
}

bool PresetModel::loadPreset(const QString &filePath)
{
    auto preset = m_saveLoader->load(filePath);
    if(!preset)
        return false;

    auto name = QFileInfo(filePath).baseName();
    addPreset(name, preset);

    return true;
}

bool PresetModel::loadAllPresets(const QString &folderPath)
{
    auto files = QDir(folderPath).entryList(QDir::Files);
    bool res = true;

    for(auto file: files)
    {
        auto fullPath = folderPath + QDir::separator() + file;
        if(!loadPreset(fullPath))
            res = false;
    }

    return res;
}

QString PresetModel::findVacantName(const QString &sourceName) const
{
    QString name = sourceName;
    int i = 1;

    while(m_storage->findPreset(name))
        name = QString("%1 (%2)").arg(sourceName).arg(i++);

    return name;
}

