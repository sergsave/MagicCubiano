#pragma once

#include <QObject>
#include <QScopedPointer>

namespace Preset {
class Storage;
class SaveLoader;
class AbstractPreset;
}

// Model provides access to list of presets
class PresetModel : public QObject
{
    Q_OBJECT

public:
    PresetModel(QObject * parent = nullptr);
    ~PresetModel();

signals:
    void changed();

public slots:
    bool setActivePreset(const QString& name);

    // This methods may change active preset
    bool addPreset(const QString& name, Preset::AbstractPreset * preset);
    bool removePreset(const QString& name);
    bool renamePreset(const QString& oldName, const QString& newName);

public:
    Preset::AbstractPreset * findPreset(const QString& name) const;
    QStringList allPresets() const;
    QString activePreset() const;
    QString findVacantName(const QString &sourceName) const;

    // loading also add preset
    bool loadPreset(const QString& filePath);
    bool loadAllPresets(const QString& folderPath);
    bool savePresets(const QString& name, const QString& filePath);

private:
    QScopedPointer<Preset::Storage> m_storage;
    QScopedPointer<Preset::SaveLoader> m_saveLoader;
    QString m_activePreset;
};

