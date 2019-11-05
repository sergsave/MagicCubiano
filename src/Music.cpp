#include "Music.h"

#include <QMap>

namespace Music {

Tone::Tone(Note note, int octave):
    note(note), octave(octave)
{}

QString Tone::toString(Format f) const
{
    static const QMap<Note, QString> notes
    {
        {C, "C"}, {D, "D"}, {E, "E"}, {F, "F"}, {G, "G"}, {A, "A"}, {B, "B"},
    };

    static const QMap<Note, QString> sharp
    {
        {C_SHARP, "C#"}, {D_SHARP, "D#"}, {F_SHARP, "F#"}, {G_SHARP, "G#"}, {A_SHARP, "A#"}
    };

    static const QMap<Note, QString> is
    {
        {C_SHARP, "Cis"}, {D_SHARP, "Dis"}, {F_SHARP, "Fis"}, {G_SHARP, "Gis"}, {A_SHARP, "Ais"}
    };

    const QString oct = QString::number(octave);

    if(notes.contains(note))
        return notes[note] + oct;

    if(f == SHARP)
        return sharp[note] + oct;

    if(f == IS)
        return is[note] + oct;

    return {};
}

Harmony::Harmony(const Tones & tones, int dur):
    tones(tones), minToneDurationMSec(dur)
{}

Tones allTonesFor(int o)
{
    using T = Tone;
    return { Tone(T::C, o), Tone(T::C_SHARP, o), Tone(T::D, o), Tone(T::D_SHARP, o), Tone(T::E, o),
             Tone(T::F, o), Tone(T::F_SHARP, o), Tone(T::G, o), Tone(T::G_SHARP, o), Tone(T::A, o),
             Tone(T::A_SHARP, o), Tone(T::B, o) };
}

Tones allTonesFor(const Interval &interval)
{
    const auto min = interval.min;
    const auto max = interval.max;

    if(min > max)
        return {};

    const auto fullOctave = Music::allTonesFor(0);

    const auto minIdx = fullOctave.indexOf(min.note);
    const auto maxIdx = fullOctave.indexOf(max.note);

    Music::Tones allTones;

    for(int o = min.octave; o <= max.octave; ++o)
    {
        int pos = 0;
        int len = -1;

        if(o == min.octave)
            pos = minIdx;

        if(o == max.octave)
            len = 1 + maxIdx - pos;

        allTones << Music::allTonesFor(o).mid(pos, len);
    }

    return allTones;
}

bool operator == (const Tone& lhs, const Tone& rhs)
{
    return lhs.note == rhs.note && lhs.octave == rhs.octave;
}

bool operator > (const Tone& lhs, const Tone& rhs)
{
    if(lhs.octave != rhs.octave)
        return lhs.octave > rhs.octave;

    return static_cast<int>(lhs.note) > static_cast<int>(rhs.note);
}

bool operator < (const Tone& lhs, const Tone& rhs)
{
    if(lhs.octave != rhs.octave)
        return lhs.octave < rhs.octave;

    return static_cast<int>(lhs.note) < static_cast<int>(rhs.note);
}



}
