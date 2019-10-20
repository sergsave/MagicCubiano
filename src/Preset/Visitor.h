#pragma once

#include "src/Instruments/Common.h"

namespace Preset {

template <class Instrument>
class TPreset;

using GuitarPreset = TPreset<Instruments::GuitarTag>;
using ElectricGuitarPreset = TPreset<Instruments::ElectricGuitarTag>;
using PianoPreset = TPreset<Instruments::PianoTag>;

class Visitor
{
public:
    virtual ~Visitor() {}
    virtual void visit(GuitarPreset&) = 0;
    virtual void visit(ElectricGuitarPreset&) = 0;
    virtual void visit(PianoPreset&) = 0;
};

class ConstVisitor
{
public:
    virtual ~ConstVisitor() {}
    virtual void visit(const GuitarPreset&) = 0;
    virtual void visit(const ElectricGuitarPreset&) = 0;
    virtual void visit(const PianoPreset&) = 0;
};

}

