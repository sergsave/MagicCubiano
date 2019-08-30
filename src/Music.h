#pragma once

#include <QString>
#include <QVector>

namespace Music {

enum class Instrument
{
    GUITAR,
    DISTORTION_GUITAR,
    PIANO,
    BEEPER
};

struct Tone
{
    // Order in frequency increasement
    enum Note { C, C_SHARP, D, D_SHARP, E, F, F_SHARP, G, G_SHARP, A, A_SHARP, B };
    // Formats: A#0 or Ais0
    enum Format { SHARP, IS};

    Tone(Note note = B, int octave = 0);

    QString toString(Format = Format::IS) const;

    Note note;
    int octave;
};

struct Interval
{
    Interval(const Tone& min = {}, const Tone& max = {}): min(min), max(max) {}
    Tone min;
    Tone max;
};

using Tones = QVector<Tone>;
Tones allTonesFor(int octave);
Tones allTonesFor(const Interval& interval);

struct Harmony
{
    Harmony() {}
    Harmony(const Tones&, int delayMSec);

    Tones tones;
    int delayMSec = 0;
};

bool operator == (const Tone& lhs, const Tone& rhs);
bool operator > (const Tone& lhs, const Tone& rhs);
bool operator < (const Tone& lhs, const Tone& rhs);

}
