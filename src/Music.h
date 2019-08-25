#pragma once

#include <QString>
#include <QVector>

namespace Music {

struct Tone
{
    enum Note { C, C_SHARP, D, D_SHARP, E, F, F_SHARP, G, G_SHARP, A, A_SHARP, B };

    Tone(Note note = B, int octave = 0);

    // Format: "A#0"
    QString toString() const;

    Note note;
    int octave;

    bool operator==(const Tone& tone)
    {
        return note == tone.note && octave == tone.octave;
    }
};

using Tones = QVector<Tone>;

Tones allTonesForOctave(int octave);

struct Harmony
{
    Harmony() {}
    Harmony(const Tones&, int delayMSec);

    Tones tones;
    int delayMSec = 0;
};



}

