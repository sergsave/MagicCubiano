#pragma once

#include "Notations.h"

namespace Instruments {

struct GuitarTag { using Notation = GuitarNotation; };
struct ElectricGuitarTag { using Notation = GuitarNotation; };
struct PianoTag { using Notation = PianoNotation; };

namespace Details {

GuitarNotation min(const GuitarTag&);
GuitarNotation max(const GuitarTag&);
Music::Tone toneFor(const GuitarNotation&);

PianoNotation min(const PianoTag&);
PianoNotation max(const PianoTag&);
Music::Tone toneFor(const PianoNotation&);

}

template <class Tag>
struct Description
{
    using Notation = typename Tag::Notation;
    static Notation min() { return Details::min(Tag()); }
    static Notation max() { return Details::max(Tag()); }
    static Music::Tone toneFor(const Notation& notation) { return Details::toneFor(notation); }
};

template<>
struct Description<ElectricGuitarTag> : public Description<GuitarTag> {};

template <typename Tag>
Music::Interval defineInterval()
{
    using Desc = Description<Tag>;
    return { Desc::toneFor(Desc::min()), Desc::toneFor(Desc::max()) };
}

}
