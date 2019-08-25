#pragma once

// Magic cube always has 6 edges
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
};

