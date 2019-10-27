#include "SettingsModel.h"

void SettingsModel::setVolume(int vol)
{
    if(vol == m_volume)
        return;

    m_volume = vol;
    emit changed();
}

int SettingsModel::volume() const
{
    return m_volume;
}
