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
 * Definition of lines and of the magic numbers that encode their attributes.
 */

#pragma once

#include "Point.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
/**
 * Types of brushes.
 *
 * Each brush defines the texture and sensitivity of strokes that are applied
 * to the canvas when the pen is used. Brush type can be selected using the
 * GUI. There is exactly one brush type associated with each line.
 *
 * | Brush Type    | Pressure  |   Speed   |   Tilt    |
 * | ------------- |:---------:|:---------:|:---------:|
 * | Ballpoint pen |     X     |           |           |
 * | Marker pen    |     X     |           |     X     |
 * | Fineliner pen |           |           |           |
 * | Sharp pencil  |           |           |           |
 * | Tilt pencil   |     X     |           |     X     |
 * | Brush         |     X     |     X     |     X     |
 *
 * Refer to <https://blog.remarkable.com/f53c8faeab77> for a visual overview
 * of the differences between the brush types.
 */
namespace Brushes
{
    enum Brushes
    {
        /**
         * Ballpoint pen.
         */
        pen1 = 2u,

        /**
         * Marker pen.
         */
        pen2 = 3u,

        /**
         * Fineliner pen.
         */
        fineliner = 4u,

        /**
         * Sharp pencil.
         */
        pencil_sharp = 7u,

        /**
         * Tilt pencil.
         */
        pencil_tilt = 1u,

        /**
         * Brush.
         */
        brush = 0u,

        /**
         * Highlighter.
         */
        highlighter = 5u,

        /**
         * Normal eraser.
         */
        rubber = 6u,

        /**
         * Maybe “clear page” or “select”?
         */
        unknown_brush = 7u,

        /**
         * Area eraser.
         */
        rubber_area = 8u
    };
}

/**
 * Shades of grey.
 *
 * Defines the color of the brush used for a line. As the reMarkable uses a
 * E Ink display, it only handles shades of grey (no colors). There is
 * exactly one color selected for each line.
 */
namespace Colors
{
    enum Colors
    {
        /**
         * Black color.
         */
        black = 0u,

        /**
         * Grey color.
         */
        grey = 1u,

        /**
         * White color.
         */
        white = 2u
    };
}

/**
 * Base brush sizes.
 *
 * Defines the base width of the brush used for a line, in pixel units. This
 * size can be further affected by the pressure and tilt parameters, for
 * brushes that support it.
 */
namespace BaseSizes
{
    /**
     * Small size.
     */
    constexpr float small = 1.875;

    /**
     * Medium size.
     */
    constexpr float mid = 2.0;

    /**
     * Large size.
     */
    constexpr float large = 2.125;
}

    /**
     * Element of a layer, containing a set of points resulting from
     * a single brush stroke.
     *
     * @see rmlab::Layer
     * @see rmlab::Point
     */
    struct Line
    {
        /**
         * Kind of brush that was selected while drawing the line.
         * @see rmlab::Brushes
         */
        int32_t brush_type;

        /**
         * Color of the brush as selected while drawing the line.
         * @see rmlab::Colors
         */
        int32_t color;

        /**
         * Attribute whose purpose is still unknown.
         */
        int32_t unknown_line_attribute;

        /**
         * Base size of the brush as selected while drawing the line.
         * @see rmlab::BaseSizes
         */
        float brush_base_size;

        /**
         * Number of points in this line.
         */
        int32_t npoints;

        /**
         * All points contained in this line, in the order they were drawn.
         */
        std::list< Point > points;
    };
}
