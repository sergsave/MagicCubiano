#include "Model.h"

Model::Model(QObject *parent):
    QObject(parent)
{
    connect(&m_settings, &SettingsModel::changed, this, &Model::settingsChanged);
    connect(&m_presets, &PresetModel::changed, this, &Model::presetsChanged);
}

SettingsModel *Model::settings()
{
    return &m_settings;
}

PresetModel *Model::presets()
{
    return &m_presets;
}

GiikerProtocol *Model::remoteProtocol()
{
    return &m_protocol;
}

const SettingsModel *Model::settings() const
{
    return &m_settings;
}

const PresetModel *Model::presets() const
{
    return &m_presets;
}

const GiikerProtocol *Model::remoteProtocol() const
{
    return &m_protocol;
}
