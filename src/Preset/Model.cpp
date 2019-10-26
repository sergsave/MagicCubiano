#include "Model.h"

#include <QFileInfo>

#include "Storage.h"
#include "SaveLoader.h"
#include "Presets.h"

namespace Preset {

Model::Model(QObject *parent)
    : QObject(parent)
    , m_storage(new Storage())
    , m_saveLoader(new SaveLoader())
{}

Model::~Model() = default;

bool Model::setActivePreset(const QString &name)
{
    if(!findPreset(name))
        return false;

    if(name == m_activePreset)
        return false;

    m_activePreset = name;
    emit changed();
    return true;
}

bool Model::addPreset(const QString &name, AbstractPreset *preset)
{
    if(!m_storage->addPreset(name, preset))
        return false;

    if(!setActivePreset(name))
        emit changed();

    return true;
}

bool Model::removePreset(const QString &name)
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

bool Model::renamePreset(const QString &oldName, const QString &newName)
{
    if(!m_storage->renamePreset(oldName, newName))
        return false;

    if(!setActivePreset(newName))
        emit changed();

    return true;
}

AbstractPreset *Model::findPreset(const QString &name) const
{
    return m_storage->findPreset(name);
}

QStringList Model::allPresets() const
{
    return m_storage->allPresetNames();
}

QString Model::activePreset() const
{
    return m_activePreset;
}

bool Model::save(const QString &name, const QString &filePath)
{
    auto preset = findPreset(name);
    if(!preset)
        return false;

    return m_saveLoader->save(filePath, preset);
}

bool Model::load(const QString &filePath)
{
    auto preset = m_saveLoader->load(filePath);
    if(!preset)
        return false;

    auto name = QFileInfo(filePath).baseName();
    addPreset(name, preset);

    return true;
}

QString Model::findVacantName(const QString &sourceName) const
{
    QString name = sourceName;
    int i = 1;

    while(m_storage->findPreset(name))
        name = QString("%1 (%2)").arg(sourceName).arg(i++);

    return name;
}

}
