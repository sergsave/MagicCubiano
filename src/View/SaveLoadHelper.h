#pragma once

#include <QObject>
#include <QScopedPointer>

class SaveLoadHelper : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadHelper(QObject *parent = nullptr);
    ~SaveLoadHelper();

    void choosePathForSaving(const QString& presetName);
    void choosePathForLoading();
    void choosePathForLoadingAll();

signals:
    void loadRequested(const QString& filePath);
    void loadAllRequested(const QString& dir);
    void saveRequested(const QString& presetName, const QString& filePath);
};
