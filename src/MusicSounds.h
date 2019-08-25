#pragma once

#include <QString>
#include <QList>

namespace Music {

struct Tone
{
    enum Note { B, C, D, E, F, G, A };
    enum Accidental { None, Sharp, Flat };

    Tone(Note note = A, Accidental acc = None, int octave = 0);

    // Format: "A#0"
    QString toString() const;

    Note note;
    Accidental accidental;
    int octave;

    bool operator==(const Tone& tone)
    {
        return note == tone.note
            && accidental == tone.accidental
            && octave == tone.octave;
    }
};

struct Harmony
{
    using Tones = QList<Tone>;

    Harmony() {}
    Harmony(const Tones&, int delayMSec);

    Tones tones;
    int delayMSec = 0;
};

}

