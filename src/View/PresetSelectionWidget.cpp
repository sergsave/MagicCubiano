#include "PresetSelectionWidget.h"

PresetSelectionWidget::PresetSelectionWidget(QWidget * parent):
    SelectionWidget(parent)
{
    setTitle("Preset: ");
    connect(this, &SelectionWidget::indexChanged, this, [this] (int idx) {
        if(idx < m_presets.size() && idx >=0 )
            emit presetChanged(m_presets.values()[idx]);
    });
}

void PresetSelectionWidget::setPresets(const QMap<QString, Preset> & presets)
{
    m_presets = presets;

    setValues(presets.keys());
}

QMap<QString, Preset> PresetSelectionWidget::presets() const
{
    return m_presets;
}
