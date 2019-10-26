#pragma once

#include "Sound/SoundGeneratorFactory.h"
#include "Preset/Presets.h"

static SoundGenerator * soundGeneratorFor(Preset::AbstractPreset * preset)
{
    class SoundGeneratorVisitor : public Preset::ConstVisitor
    {
        using T = Instruments::Type;
    public:
        void visit(const Preset::GuitarPreset&)  override { m_type = T::GUITAR; }
        void visit(const Preset::ElectricGuitarPreset&) override { m_type = T::ELECTRIC_GUITAR; }
        void visit(const Preset::PianoPreset&) override { m_type = T::PIANO; }

        SoundGenerator * get() const { return createSoundGenerator(m_type); }

    private:
        T m_type;
    } visitor;

    preset->acceptVisitor(visitor);
    return visitor.get();
}


