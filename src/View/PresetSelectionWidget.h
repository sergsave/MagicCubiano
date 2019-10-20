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

    QString currentPreset() const;

    bool remove(const QString&);
    bool add(const QString& presetName, const QString& instrumentName);

    bool isEmpty() const;

signals:
    void emptyStateChanged(bool isEmpty);

    void presetSelected(const QString&);
    void presetEditRequested(const QString&);
    void presetRenamed(const QString& oldName, const QString& newName);

private:
    void changeIndex(int newIdx);
    void checkForEmpty();
    void updateDirectionButtonsState();
    void updateState();

private:
    QStringList m_presets;
    QMap<QString, QString> m_presets2instruments;
    int m_index = 0;
    QScopedPointer<Ui::PresetSelectionWidget> m_ui;
};
