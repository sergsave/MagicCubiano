#pragma once

#include <QJsonObject>
#include "src/Instruments/Common.h"

namespace Preset {

// TODO: result check

void serializeNotation(const Instruments::GuitarNotation& notation, QJsonObject& obj);
void deserializeNotation(Instruments::GuitarNotation& notation, const QJsonObject& obj);

void serializeNotation(const Instruments::PianoNotation& notation, QJsonObject& obj);
void deserializeNotation(Instruments::PianoNotation& notation, const QJsonObject& obj);

}
