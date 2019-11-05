#pragma once

#include <QList>

// Magic cube always has 6 edges
// Attention! enums are used for serialization
struct CubeEdge
{
    enum Color
    {
        GREEN,
        BLUE,
        RED,
        ORANGE,
        YELLOW,
        WHITE
    };

    enum Rotation
    {
        CLOCKWIZE,
        ANTICLOCKWIZE
    };

    CubeEdge(Color color = GREEN, Rotation rotation = CLOCKWIZE) :
        color(color), rotation(rotation) {}

    Color color;
    Rotation rotation;

    static QList<CubeEdge::Color> allColors()
    {
        return { GREEN, BLUE, RED, ORANGE, YELLOW, WHITE };
    }

    static QList<CubeEdge::Rotation> allRotations()
    {
        return { CLOCKWIZE, ANTICLOCKWIZE };
    }

    bool operator == (const CubeEdge& ce) const
    {
        return this->rotation == ce.rotation && this->color == ce.color;
    }
};

