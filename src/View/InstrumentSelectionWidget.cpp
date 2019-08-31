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
        { Ins::DISTORTION_GUITAR, "Distortion \nGuitar" },
        { Ins::PIANO, "Piano"},
        { Ins::BEEPER, "Beeper"}
    };

    if(!map.contains(ins))
        assert(!"unknow instrument");

    return map.value(ins, {});
}

}

InstrumentSelectionWidget::InstrumentSelectionWidget(QWidget * parent) :
    QWidget(parent),
    m_ui(new Ui::InstrumentSelectionWidget),
    m_instruments(Configuration::supportedInstruments())
{
    assert(!m_instruments.empty());
    m_ui->setupUi(this);

    connect(m_ui->leftButton, &QAbstractButton::clicked, this, &InstrumentSelectionWidget::increase);
    connect(m_ui->rightButton, &QAbstractButton::clicked, this, &InstrumentSelectionWidget::decrease);

    onIndexUpdated();
}

QList<Music::Instrument> InstrumentSelectionWidget::supportedInstruments() const
{
    return m_instruments;
}

void InstrumentSelectionWidget::setInstrumentType(Music::Instrument type)
{
    if(!m_instruments.contains(type))
    {
        assert(!"not supported");
        return;
    }

    m_index = m_instruments.indexOf(type);
}

Music::Instrument InstrumentSelectionWidget::instrumentType() const
{
    return m_instruments[m_index];
}

void InstrumentSelectionWidget::increase()
{
    if(++m_index == m_instruments.size())
        m_index = 0;

    onIndexUpdated();
}

void InstrumentSelectionWidget::decrease()
{
    if(m_index-- == 0)
        m_index = m_instruments.size() - 1;

    onIndexUpdated();
}

void InstrumentSelectionWidget::onIndexUpdated()
{
    emit instrumentTypeChanged(m_instruments[m_index]);

    m_ui->leftButton->setEnabled(m_index > 0);
    m_ui->rightButton->setEnabled(m_index < (m_instruments.size() - 1));

    m_ui->modeLabel->setText(instrumentToString(m_instruments[m_index]) + " mode");
}

InstrumentSelectionWidget::~InstrumentSelectionWidget() = default;
