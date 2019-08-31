#pragma once

#include <QList>
#include "SelectionWidget.h"

#include "src/Preset.h"

class PresetSelectionWidget: public SelectionWidget
{
    Q_OBJECT
public:
    PresetSelectionWidget(QWidget * parent = nullptr);

    void setPresets(const QMap<QString, Preset>&);
    QMap<QString, Preset> presets() const;

    Preset preset() const;

signals:
    void presetChanged(const Preset& preset);

private:
    QMap<QString, Preset> m_presets;
};
