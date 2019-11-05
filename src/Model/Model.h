#pragma once

#include <QObject>

#include "PresetModel.h"
#include "SettingsModel.h"
#include "src/Protocol/GiikerProtocol.h"

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
    GiikerProtocol * remoteProtocol();

    SettingsModel const * settings() const;
    PresetModel const * presets() const;
    GiikerProtocol const * remoteProtocol() const;

private:
    SettingsModel m_settings;
    PresetModel m_presets;
    GiikerProtocol m_protocol;
};
