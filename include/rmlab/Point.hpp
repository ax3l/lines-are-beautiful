#pragma once

#include <cmath>


namespace rmlab
{
namespace ranges
{
    enum Coords
    {
        minX = 0u,
        maxX = 1404u,
        minY = 0u,
        maxY = 1872u,
    };

    constexpr float minP = 0.0;
    constexpr float maxP = 1.0;

    constexpr float minRot = -M_PI / 2.0;
    constexpr float maxRot =  M_PI / 2.0;
}

    struct Point
    {
        // .lines info
        float x;
        float y;
        float pressure;
        float rotX;
        float rotY;
    };
}
