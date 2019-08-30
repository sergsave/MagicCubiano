#pragma once

#include "SampleGenerator.h"

class GuitarGenerator : public SampleGenerator
{
public:
    GuitarGenerator(const Music::Interval&, QObject * parent = nullptr);
};

class DistortionGuitarGenerator : public SampleGenerator
{
public:
    DistortionGuitarGenerator(const Music::Interval&, QObject * parent = nullptr);
};
