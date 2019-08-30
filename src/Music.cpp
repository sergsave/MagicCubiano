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

Harmony::Harmony(const Tones & tones, int delayMSec):
    tones(tones), delayMSec(delayMSec)
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
    Music::Tones allTones;

    auto min = interval.min;
    auto max = interval.max;

    auto fullOctave = Music::allTonesFor(0);

    auto minIdx = fullOctave.indexOf(min.note);
    auto maxIdx = fullOctave.indexOf(max.note);

    allTones << Music::allTonesFor(min.octave).mid(minIdx);

    for(int octave = (min.octave + 1); octave < (max.octave - 1); ++octave)
        allTones << Music::allTonesFor(octave);

    allTones << Music::allTonesFor(max.octave).mid(0, fullOctave.size() - maxIdx);

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
