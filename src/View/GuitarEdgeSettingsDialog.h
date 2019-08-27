#pragma once

#include <QScopedPointer>
#include <QVector>

#include "EdgeSettingsDialog.h"

namespace Ui {
class GuitarEdgeSettingsDialog;
}

class StringWidget;

class GuitarEdgeSettingsDialog : public EdgeSettingsDialog
{
    Q_OBJECT

public:
    explicit GuitarEdgeSettingsDialog(const QPixmap& icon, QWidget *parent = nullptr);
    ~GuitarEdgeSettingsDialog();

    Music::Harmony harmony() const override;

private:
    static const int m_fretQuantity = 15;
    static const int m_stringQuantity = 6;

    QScopedPointer<Ui::GuitarEdgeSettingsDialog> m_ui;
    QVector<StringWidget*> m_strings;

    // TODO: dynamic change
    static const int m_delayMSec = 0;
};
