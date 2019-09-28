#include "PresetSelectionWidget.h"

PresetSelectionWidget::PresetSelectionWidget(QWidget * parent):
    SelectionWidget(parent)
{
    setTitle("Preset: ");
    connect(this, &SelectionWidget::indexChanged, this, [this] (int idx) {
        if(idx < m_presets.size() && idx >=0 )
            emit presetChanged(m_presets[idx]);
    });
}

void PresetSelectionWidget::addPreset(const NamedPreset& preset)
{
    m_presets.append(preset);

    QStringList values;
    for(auto& p: m_presets) values.append(p.first);

    setValues(values);
    // Last preset is active
    setIndex(maxIndex());
}

PresetSelectionWidget::NamedPreset PresetSelectionWidget::preset() const
{
    return m_presets.value(index());
}

QList<PresetSelectionWidget::NamedPreset> PresetSelectionWidget::presets() const
{
    return m_presets;
}
