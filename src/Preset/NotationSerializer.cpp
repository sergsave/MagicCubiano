#include "NotationSerializer.h"

namespace {

const QString g_stringKey = "string";
const QString g_fretKey = "fret";
const QString g_noteKey = "note";
const QString g_octaveKey = "octave";

}

namespace Preset {

void serializeNotation(const Instruments::GuitarNotation& notation, QJsonObject& obj)
{
    obj[g_stringKey] = notation.string;
    obj[g_fretKey] = notation.fret;
}

void deserializeNotation(Instruments::GuitarNotation& notation, const QJsonObject& obj)
{
    notation.string = obj[g_stringKey].toInt();
    notation.fret = obj[g_fretKey].toInt();
}

void serializeNotation(const Instruments::PianoNotation& notation, QJsonObject &obj)
{
    obj[g_noteKey] = static_cast<int>(notation.note);
    obj[g_octaveKey] = notation.octave;
}

void deserializeNotation(Instruments::PianoNotation& notation, const QJsonObject& obj)
{
    notation.note = static_cast<Music::Tone::Note>(obj[g_noteKey].toInt());
    notation.octave = obj[g_octaveKey].toInt();
}

}

