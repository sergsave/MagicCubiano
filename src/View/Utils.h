#pragma once

#include <QString>
#include <QColor>
#include <QLabel>

#include "src/Instruments/Common.h"
#include "src/CubeEdge.h"

namespace Preset {
class AbstractPreset;
}

QString instrumentName(Instruments::Type type);
QString instrumentName(Preset::AbstractPreset * preset);

QString tonesToString(const Music::Tones& tones);

struct EdgeAppearance
{
    EdgeAppearance(const QColor& col, const QString& iconPath)
        : color(col), iconPath(iconPath) {}
    QColor color;
    QString iconPath;
};

EdgeAppearance appearanceFor(const CubeEdge& edge);

void setLabelPixmap(QLabel *, const QString& path);

class QMenu;
class QAbstractButton;

void bindMenu(QMenu * menu, QAbstractButton*);


