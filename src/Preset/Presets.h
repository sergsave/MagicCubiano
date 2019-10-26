#pragma once

#include <QString>
#include <QMap>

#include "Visitor.h"
#include "src/CubeEdge.h"

namespace Preset {

template <class InstrumentTag>
struct Unit
{
    int minDurationMSec = 0;
    QList<typename InstrumentTag::Notation> notations;
};

template <class InstrumentTag>
using Data = QMap<CubeEdge::Rotation, QMap<CubeEdge::Color, Unit<InstrumentTag>>>;

class AbstractPreset
{
public:
    virtual ~AbstractPreset() {}
    virtual void acceptVisitor(Visitor& v) = 0;
    virtual void acceptVisitor(ConstVisitor& v) const = 0;
    // TODO: Remove toHarmony??
    virtual Music::Harmony toHarmony(const CubeEdge&) const = 0;
};

template <class Instrument>
class TPreset : public AbstractPreset
{
public:
    void acceptVisitor(Visitor& v) override { v.visit(*this); }
    void acceptVisitor(ConstVisitor& cv) const override { cv.visit(*this); }

    Music::Harmony toHarmony(const CubeEdge& ce) const override
    {
        Music::Harmony ret;
        auto unit = m_data[ce.rotation][ce.color];

        ret.minToneDurationMSec = unit.minDurationMSec;
        for(auto notation: unit.notations)
            ret.tones.append(Desc::toneFor(notation));
        return ret;
    }

    Data<Instrument> data() const { return m_data; }
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
