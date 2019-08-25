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

Tones allTonesForOctave(int n)
{
    using T = Tone;
    return { Tone(T::C, n), Tone(T::C_SHARP, n), Tone(T::D, n), Tone(T::D_SHARP, n), Tone(T::E, n),
             Tone(T::F, n), Tone(T::F_SHARP, n), Tone(T::G, n), Tone(T::G_SHARP, n), Tone(T::A, n),
             Tone(T::A_SHARP, n), Tone(T::B, n) };
}

}
