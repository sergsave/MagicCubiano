#pragma once

#include <QObject>
#include <QScopedPointer>

class SaveLoadHelper : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadHelper(QObject *parent = nullptr);
    ~SaveLoadHelper();

    void startSaving(const QString& presetName);
    void startLoading();

signals:
    void loadRequested(const QString& filePath);
    void saveRequested(const QString& presetName, const QString& filePath);

};
