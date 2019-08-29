#pragma once

#include "SampleGenerator.h"

class GuitarGenerator : public SampleGenerator
{
public:
    using SampleGenerator::SampleGenerator;

    QString resourceFor(const Music::Tone& tone) const override
    {
        auto copy = tone;
        copy.octave -= 2;

        QString prefix = ":/sounds/guitar/";
        QString postfix = ".wav";

        // TODO: call of base class, if not exist

        return prefix + copy.toString(Music::Tone::Format::IS) + postfix;
    }
};
