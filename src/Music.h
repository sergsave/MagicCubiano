#pragma once

#include <QString>
#include <QVector>

namespace Music {

struct Tone
{
    enum Note { C, C_SHARP, D, D_SHARP, E, F, F_SHARP, G, G_SHARP, A, A_SHARP, B };
    // Formats: A#0 or Ais0
    enum Format { SHARP, IS};

    Tone(Note note = B, int octave = 0);

    QString toString(Format = Format::IS) const;

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

