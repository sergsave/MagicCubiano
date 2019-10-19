#pragma once

#include "Sound/SoundGeneratorFactory.h"
#include "Preset/Model.h"

static SoundGenerator * soundGeneratorFor(Preset::AbstractPreset * preset)
{
    class SoundGeneratorVisitor : public Preset::Visitor
    {
        using T = Instruments::Type;
    public:
        void visit(Preset::GuitarPreset *) override { m_type = T::GUITAR; }
        void visit(Preset::ElectricGuitarPreset *) override { m_type = T::ELECTRIC_GUITAR; }
        void visit(Preset::PianoPreset *) override { m_type = T::PIANO; }

        SoundGenerator * get() const { return createSoundGenerator(m_type); }

    private:
        T m_type;
    } visitor;

    preset->acceptVisitor(&visitor);
    return visitor.get();
}


