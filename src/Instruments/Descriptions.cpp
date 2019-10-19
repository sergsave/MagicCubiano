#include "Descriptions.h"
#include <QVector>
#include <cassert>

namespace {

// http://static1.squarespace.com/static/57a08f8a414fb546e8e35178/587dae1f9f7456b5f7fc5413/59e456f08dd041f0a6c9ed93/1508217399278/Guitar_fret_notes_and_frequency_hz.png?format=1500w

using namespace Music;
struct TonesTable
{
    static const QVector<Tones> data;
    static const int stringQuantity = 6;
    static const int fretQuantity = 21;

    TonesTable()
    {
        assert(data.size() == stringQuantity);
        for(auto tones: data)
            assert(tones.size() == fretQuantity);

    }
};

using T = Tone;
const QVector<Tones> TonesTable::data{
    allTonesFor( {Tone(T::E, 4), Tone(T::C,       6) }),
    allTonesFor( {Tone(T::B, 3), Tone(T::G,       5) }),
    allTonesFor( {Tone(T::G, 3), Tone(T::D_SHARP, 5) }),
    allTonesFor( {Tone(T::D, 3), Tone(T::A_SHARP, 4) }),
    allTonesFor( {Tone(T::A, 2), Tone(T::F,       4) }),
    allTonesFor( {Tone(T::E, 2), Tone(T::C,       4) })
};

TonesTable g_tonesTable;

}

namespace Instruments {

GuitarNotation Details::min(const GuitarTag &)
{
    return { 0, 0 };
}

GuitarNotation Details::max(const GuitarTag &)
{
    return { g_tonesTable.stringQuantity - 1, g_tonesTable.fretQuantity - 1 };
}

Tone Details::toneFor(const GuitarNotation & notation)
{
    int string = notation.string;
    int fret = notation.fret;

    if(string < 0 || string >= g_tonesTable.data.size())
    {
        assert(!"out of range");
        return {};
    }

    const auto tonesForString = g_tonesTable.data[string];

    if(fret < 0 || fret >= tonesForString.size())
    {
        assert(!"out of range");
        return {};
    }

    return tonesForString[fret];
}

PianoNotation Details::min(const PianoTag &) { return Music::Tone(Music::Tone::A, 0); }

PianoNotation Details::max(const PianoTag &) { return Music::Tone(Music::Tone::C, 8); }

Tone Details::toneFor(const PianoNotation &notation) { return notation; }

}
