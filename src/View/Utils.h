#pragma once

#include <QString>
#include <QColor>

#include "src/Instruments/Common.h"
#include "src/CubeEdge.h"

namespace Preset {
class AbstractPreset;
}

QString instrumentName(Instruments::Type type);

QString instrumentName(Preset::AbstractPreset * preset);

struct EdgeAppearance
{
    EdgeAppearance(const QColor& col, const QString& iconPath)
        : color(col), iconPath(iconPath) {}
    QColor color;
    QString iconPath;
};

EdgeAppearance appearanceFor(const CubeEdge& edge);
