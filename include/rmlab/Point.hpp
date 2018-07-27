/* Copyright 2017-2018 Axel Huebl
 *
 * This file is part of lines-are-beautiful.
 *
 * lines-are-beautiful is free software: you can redistribute it and/or modify
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

/**
 * @file
 * Definition of points and of the magic numbers that encode their attributes.
 */

#pragma once

#include <cmath>


namespace rmlab
{
namespace ranges
{
    /**
     * Rectangle in which the points are located.
     */
    enum Coords
    {
        /**
         * Minimum value of the X coordinate, or X coordinate
         * of the left-side of the bounding rectangle.
         */
        minX = 0u,

        /**
         * Maximum value of the X coordinate, or X coordinate
         * of the right-side of the bounding rectangle.
         */
        maxX = 1404u,

        /**
         * Minimum value of the Y coordinate, or Y coordinate
         * of the upper-side of the bounding rectangle.
         */
        minY = 0u,

        /**
         * Maximum value of the Y coordinate, or Y coordinate
         * of the bottom-side of the bounding rectangle.
         */
        maxY = 1872u,
    };

    /**
     * Minimum pressure value.
     */
    constexpr float minP = 0.0;

    /**
     * Maximum pressure value.
     */
    constexpr float maxP = 1.0;

    /**
     * Minimum rotation value of the pen, in radians.
     */
    constexpr float minRot = -M_PI / 2.0;

    /**
     * Maximum rotation value of the pen, in radians.
     */
    constexpr float maxRot =  M_PI / 2.0;
}

    /**
     * Element of a line. A line is made up of a sequence of points
     * sampled at a constant interval.
     */
    struct Point
    {
        /**
         * Position on the X-axis, relative to the upper-left corner
         * of the device’s screen. This value is expressed in pixels
         * and is comprised between rmlab::ranges::Coords::minX and
         * rmlab::ranges::Coords::maxX.
         */
        float x;

        /**
         * Position on the Y-axis, relative to the upper-left corner
         * of the device’s screen. This value is expressed in pixels
         * and is comprised between rmlab::ranges::Coords::minY and
         * rmlab::ranges::Coords::maxY.
         */
        float y;

        /**
         * Pressure that was being applied on the screen with the pen when
         * this point was sampled. This value is comprised between
         * rmlab::ranges::minP and rmlab::ranges::maxP.
         */
        float pressure;

        /**
         * Rotation of the pen around the X axis when this point was sampled.
         * This rotation is expressed in radians and comprised between
         * rmlab::ranges::minRot and rmlab::ranges::maxRot.
         */
        float rotX;

        /**
         * Rotation of the pen around the Y axis when this point was sampled.
         * This rotation is expressed in radians and comprised between
         * rmlab::ranges::minRot and rmlab::ranges::maxRot.
         */
        float rotY;
    };
}
