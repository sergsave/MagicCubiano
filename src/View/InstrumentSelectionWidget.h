#pragma once

#include <QWidget>
#include <QList>
#include <QScopedPointer>

#include "src/Music.h"

namespace Ui {
class InstrumentSelectionWidget;
}

class InstrumentSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentSelectionWidget(QWidget *parent = nullptr);
    ~InstrumentSelectionWidget();

    QList<Music::Instrument> supportedInstruments() const;

    void setInstrumentType(Music::Instrument);
    Music::Instrument instrumentType() const;

signals:
    void instrumentTypeChanged(Music::Instrument);

private slots:
    void increase();
    void decrease();

private:
    void onIndexUpdated();

private:
    QScopedPointer<Ui::InstrumentSelectionWidget> m_ui;

    QList<Music::Instrument> m_instruments;
    int m_index = 0;
};

