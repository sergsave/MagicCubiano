#pragma once

#include "SampleGenerator.h"

class GuitarGenerator : public SampleGenerator
{
public:
    GuitarGenerator(QObject * parent = nullptr);
};

class ElectricGuitarGenerator : public SampleGenerator
{
public:
    ElectricGuitarGenerator(QObject * parent = nullptr);
};
