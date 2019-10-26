#pragma once

#include <cassert>
#include <QString>
#include <QMap>

#include "src/Preset/Presets.h"

// TODO: Common utils
class InstrumentTypeFetchVisitor : public Preset::ConstVisitor
{
    using T = Instruments::Type;
public:
    void visit(const Preset::GuitarPreset&) override { m_type = T::GUITAR; }
    void visit(const Preset::ElectricGuitarPreset&) override { m_type = T::ELECTRIC_GUITAR; }
    void visit(const Preset::PianoPreset&) override { m_type = T::PIANO; }

    Instruments::Type type() const { return m_type; }

private:
    T m_type;
};

static QString instrumentName(Instruments::Type type)
{
    using T = Instruments::Type;

    switch (type)
    {
    case T::GUITAR:
        return "Guitar";
    case T::ELECTRIC_GUITAR:
        return "Electric guitar";
    case T::PIANO:
        return "Piano";
    default:
        assert(!"unreal");
        return {};
    }
}

static QString instrumentName(Preset::AbstractPreset * preset)
{
    InstrumentTypeFetchVisitor visitor;
    preset->acceptVisitor(visitor);
    return instrumentName(visitor.type());
}
