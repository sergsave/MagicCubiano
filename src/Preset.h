#pragma once

#include <QString>
#include <QMap>
#include "Music.h"
#include "CubeEdge.h"

using Preset = QMap<CubeEdge::Rotation, QMap<CubeEdge::Color, Music::Harmony>>;

QByteArray presetToJson(const Preset&);
Preset presetFromJson(const QByteArray&, bool& ok);
