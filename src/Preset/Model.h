#pragma once

#include <QString>
#include <QMap>

#include "Visitor.h"
#include "src/CubeEdge.h"

namespace Preset {

template <class InstrumentTag>
struct Unit
{
    int duration = 0;
    typename InstrumentTag::Notation notation;
};

template <class InstrumentTag>
using Units = QList<Unit<InstrumentTag>>;

template <class InstrumentTag>
using Data = QMap<CubeEdge::Rotation, QMap<CubeEdge::Color, Units<InstrumentTag>>>;

class AbstractPreset
{
public:
    virtual ~AbstractPreset() {}
    virtual void acceptVisitor(Visitor * v) = 0;
    // TODO: Remove toHarmony??
    virtual Music::Harmony toHarmony(const CubeEdge&) const = 0;
};

template <class Instrument>
class TPreset : public AbstractPreset
{
public:
    void acceptVisitor(Visitor * v) override { v->visit(this); }

    Music::Harmony toHarmony(const CubeEdge& ce) const override
    {
        Music::Harmony ret;
        for(auto unit: m_data[ce.rotation][ce.color])
            ret.tones.append(Desc::toneFor(unit.notation));
        return ret;
    }

    Data<Instrument> data() { return m_data; }
    void setData(const Data<Instrument>& data) { m_data = data; }

private:
    using Desc = Instruments::Description<Instrument>;
    Data<Instrument> m_data;
};

static AbstractPreset * createPreset(Instruments::Type type)
{
    using T = Instruments::Type;
    switch (type)
    {
    case T::GUITAR:
        return new GuitarPreset();
    case T::ELECTRIC_GUITAR:
        return new ElectricGuitarPreset();
    case T::PIANO:
        return new PianoPreset();
    default:
        return nullptr;
    }
}

}
