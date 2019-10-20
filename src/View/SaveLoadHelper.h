#pragma once

#include <QObject>
#include <QScopedPointer>

namespace Preset {
class Storage;
class SaveLoader;
}

class SaveLoadHelper : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadHelper(Preset::Storage * storage, QObject *parent = nullptr);
    ~SaveLoadHelper();

signals:
    void presetLoaded(const QString& name);
    void presetLoadFailed(const QString& name);

public slots:
    void load();
    void save(const QString& name) const;

private:
    Preset::Storage * m_storage = nullptr;
    QScopedPointer<Preset::SaveLoader> m_saveLoader;
};
