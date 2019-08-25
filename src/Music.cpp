#include "Music.h"

#include <QMap>

namespace Music {

Tone::Tone(Note note, int octave):
    note(note), octave(octave)
{}

QString Tone::toString() const
{
    const QMap<Note, QString> notes
    {
        {C, "C"}, {D, "D"}, {E, "E"}, {F, "F"}, {G, "G"}, {A, "A"}, {B, "B"},
        {C_SHARP, "C#"}, {D_SHARP, "D#"}, {F_SHARP, "F#"}, {G_SHARP, "G#"}, {A_SHARP, "A#"}
    };

    return notes[note] + QString::number(octave);
}

Harmony::Harmony(const Tones & tones, int delayMSec):
    tones(tones), delayMSec(delayMSec)
{}

Tones allTonesForOctave(int n)
{
    using T = Tone;
    return { Tone(T::C, n), Tone(T::C_SHARP, n), Tone(T::D, n), Tone(T::D_SHARP, n), Tone(T::E, n),
             Tone(T::F, n), Tone(T::F_SHARP, n), Tone(T::A, n), Tone(T::A_SHARP, n), Tone(T::B, n) };
}

}
