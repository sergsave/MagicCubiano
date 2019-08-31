#include "InstrumentSelectionWidget.h"
#include "ui_InstrumentSelectionWidget.h"

#include <cassert>
#include <QString>
#include <QMap>

#include <src/Configuration.h>

namespace  {

QString instrumentToString(Music::Instrument ins)
{
    using Ins = Music::Instrument;

    static const QMap<Ins, QString> map
    {
        { Ins::GUITAR, "Guitar" },
        { Ins::DISTORTION_GUITAR, "Distortion Guitar" },
        { Ins::PIANO, "Piano"},
        { Ins::BEEPER, "Beeper"}
    };

    if(!map.contains(ins))
        assert(!"unknow instrument");

    return map.value(ins, {});
}

}

InstrumentSelectionWidget::InstrumentSelectionWidget(QWidget * parent) :
    SelectionWidget(parent),
    m_instruments(Configuration::supportedInstruments())
{
    assert(!m_instruments.empty());

    setTitle("Mode: ");

    QStringList values;
    for(auto ins: m_instruments)
        values.append(instrumentToString(ins));
    setValues(values);

    connect(this, &SelectionWidget::indexChanged, this, [this] (int idx) {
        if(idx < m_instruments.size() && idx >= 0) emit instrumentTypeChanged(m_instruments[idx]);
    });
}

QList<Music::Instrument> InstrumentSelectionWidget::instrumentTypes() const
{
    return m_instruments;
}

Music::Instrument InstrumentSelectionWidget::instrumentType() const
{
    assert(!m_instruments.empty());
    return m_instruments[index()];
}
