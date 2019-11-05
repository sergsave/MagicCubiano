#pragma once

#include <QDialog>
#include <QScopedPointer>
#include <QStringList>
#include <QMap>

namespace Ui {
class PresetListDialog;
}

class PresetListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PresetListDialog(QWidget *parent = 0);
    ~PresetListDialog();

    void setPresetList(const QStringList&);

    QMap<QString, QString> renamedPresets() const;
    QStringList removedPresets() const;

private:
    bool isValidName(const QString&) const;

private:
    QScopedPointer<Ui::PresetListDialog> m_ui;

    QStringList m_displayedPresets;
    QStringList m_removedPresets;
    QMap<QString, QString> m_renamedPresets;
};
