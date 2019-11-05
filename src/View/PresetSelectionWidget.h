#pragma once

#include <QWidget>
#include <QList>
#include <QMap>
#include <QScopedPointer>

namespace Ui {
class PresetSelectionWidget;
}

class PresetSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PresetSelectionWidget(QWidget *parent = 0);
    ~PresetSelectionWidget();

    // Reset selected preset to first
    void setPresets(const QStringList& presets);
    QStringList presets() const;

    void setPresetAdditionalInfo(const QString& preset, const QString& info);
    QString presetAdditionalInfo(const QString& preset) const;

    QString selectedPreset() const;

public slots:
    void setSelectedPreset(const QString&);

signals:
    void presetSelected(const QString&);
    void presetOpenRequested(const QString&);

private:
    void changeIndex(int newIdx, bool forceChanged = false);
    void updateDirectionButtonsState();
    void updateState();

private:
    QStringList m_presets;
    QMap<QString, QString> m_presets2info;
    int m_index = 0;
    QScopedPointer<Ui::PresetSelectionWidget> m_ui;
};
