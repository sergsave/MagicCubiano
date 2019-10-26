#pragma once

#include <QObject>
#include <QScopedPointer>

namespace Preset {

class Storage;
class SaveLoader;
class AbstractPreset;

// Model provides access to list of presets
class Model : public QObject
{
    Q_OBJECT

public:
    Model(QObject * parent = nullptr);
    ~Model();

public slots:
    bool setActivePreset(const QString& name);

    // This methods may change active preset
    bool addPreset(const QString& name, AbstractPreset * preset);
    bool removePreset(const QString& name);
    bool renamePreset(const QString& oldName, const QString& newName);

signals:
    void changed();

public:
    AbstractPreset * findPreset(const QString& name) const;
    QStringList allPresets() const;
    QString activePreset() const;
    QString findVacantName(const QString &sourceName) const;

    // loading also add preset
    bool load(const QString& filePath);
    bool save(const QString& name, const QString& filePath);

private:
    QScopedPointer<Storage> m_storage;
    QScopedPointer<SaveLoader> m_saveLoader;
    QString m_activePreset;
};

}

