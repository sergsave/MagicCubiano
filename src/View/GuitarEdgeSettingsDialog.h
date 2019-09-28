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
    explicit GuitarEdgeSettingsDialog(const Music::Interval&, QWidget *parent = nullptr);
    ~GuitarEdgeSettingsDialog();

    Music::Harmony harmony() const override;
    void setHarmony(const Music::Harmony&) override;

    void setIcon(const QPixmap &pm) override;

private:
    int maxFret() const { return m_fretQuantity - 1; }
    int maxString() const { return m_stringQuantity - 1; }

private:
    static const int m_fretQuantity = 20; // with 0 open
    static const int m_stringQuantity = 6;

    QScopedPointer<Ui::GuitarEdgeSettingsDialog> m_ui;
    QVector<StringWidget*> m_strings;

    // TODO: dynamic change
    static const int m_delayMSec = 0;
};
