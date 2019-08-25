#include "MusicSounds.h"

#include <QMap>

namespace Music {

Tone::Tone(Note note, Accidental acc, int octave):
    note(note), accidental(acc), octave(octave)
{}

QString Tone::toString() const
{
    const QMap<Note, QString> notes { {B, "B"}, {C, "C"}, {D, "D"}, {E, "E"}, {F, "F"}, {G, "G"}, {A, "A"} };
    const QMap<Accidental, QString> accidentals { {None, ""}, {Sharp, "#"}, {Flat, "b"} };

    return notes[note] + accidentals[accidental] + QString::number(octave);
}

Harmony::Harmony(const Tones & tones, int delayMSec):
    tones(tones), delayMSec(delayMSec)
{}

}
