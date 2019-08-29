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
    explicit GuitarEdgeSettingsDialog(QWidget *parent = nullptr);
    ~GuitarEdgeSettingsDialog();

    Music::Harmony harmony() const override;
    void setHarmony(const Music::Harmony&) override;

    void setIcon(const QPixmap &pm) override;

private:
    static const int m_fretQuantity = 16;
    static const int m_stringQuantity = 6;

    QScopedPointer<Ui::GuitarEdgeSettingsDialog> m_ui;
    QVector<StringWidget*> m_strings;

    // TODO: dynamic change
    static const int m_delayMSec = 0;
};
