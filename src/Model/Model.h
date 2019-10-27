#pragma once

#include <QObject>

#include "PresetModel.h"
#include "SettingsModel.h"

class Model: public QObject
{
    Q_OBJECT
public:
    Model(QObject * parent = nullptr);

signals:
    void settingsChanged();
    void presetsChanged();

public:
    SettingsModel * settings();
    PresetModel * presets();

private:
    SettingsModel m_settings;
    PresetModel m_presets;
};
