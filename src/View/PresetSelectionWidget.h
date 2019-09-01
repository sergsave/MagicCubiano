#pragma once

#include <QList>
#include "SelectionWidget.h"

#include "src/Preset.h"

class PresetSelectionWidget: public SelectionWidget
{
    Q_OBJECT
public:
    using NamedPreset = QPair<QString, Preset>;

    PresetSelectionWidget(QWidget * parent = nullptr);

    void addPreset(const NamedPreset&);

    NamedPreset preset() const;
    QList<NamedPreset> presets() const;

signals:
    void presetChanged(const NamedPreset& preset);

private:
    QList<NamedPreset> m_presets;
};
