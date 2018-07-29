/* Copyright 2017-2018 Axel Huebl
 *
 * This file is part of lines-are-beautiful.
 *
 * lines-are-beautiful is free software: you can edistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lines-are-beautiful is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lines-are-beautiful.
 * If not, see <http://www.gnu.org/licenses/>.
 */

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

    inline Point make_point(
        float x,
        float y,
        float pressure,
        float rotX,
        float rotY
    )
    {
        Point newPoint;
        newPoint.x = x;
        newPoint.y = y;
        newPoint.pressure = pressure;
        newPoint.rotX = rotX;
        newPoint.rotY = rotY;
        return newPoint;
    }
}
