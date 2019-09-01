#pragma once

#include <QList>

#include "SelectionWidget.h"
#include "src/Music.h"

class InstrumentSelectionWidget : public SelectionWidget
{
    Q_OBJECT

public:
    explicit InstrumentSelectionWidget(QWidget *parent = nullptr);

    Music::Instrument instrumentType() const;
    QList<Music::Instrument> instrumentTypes() const;

signals:
    void instrumentTypeChanged(Music::Instrument);

private:
    QList<Music::Instrument> m_instruments;
};

