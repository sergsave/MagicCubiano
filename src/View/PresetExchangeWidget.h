#pragma once

#include <QWidget>

#include <src/Preset.h>
#include <functional>

class PresetExchangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PresetExchangeWidget(QWidget *parent = nullptr);

    void setPresetCompositor(const std::function<Preset()>& compositor);

signals:
    void presetImported(const QString& name, const Preset& preset);
    void presetImportFailed(const QString& name);

private slots:
    void onSaveClicked();
    void onLoadClicked();

private:
    std::function<Preset()> m_compositor;
};
