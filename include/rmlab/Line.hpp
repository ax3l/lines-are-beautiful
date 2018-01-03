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

#include "Point.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
namespace Brushes
{
    // see: https://blog.remarkable.com/how-to-find-your-perfect-writing-instrument-for-notetaking-on-remarkable-f53c8faeab77
    enum Brushes
    {
        // pens
        pen1 = 2u,          // GUI: 1-1 "ballpoint"
        pen2 = 3u,          // GUI: 1-2 "marker"
        fineliner = 4u,     // GUI: 1-3 "fineliner"
        // pencils
        pencil_sharp = 7u,  // GUI: 2-1 "sharp pencil"
        pencil_tilt = 1u,   // GUI: 2-2 "tilt pencil"
        // other
        brush = 0u,         // GUI: 3 "brush"
        highlighter = 5u,   // GUI: 5 (always color 0)
        unknown_brush = 6u  // not in the GUI yet
    };
}
namespace Colors
{
    enum Colors
    {
        black = 0u,
        grey = 1u,
        white = 2u
    };
}

namespace BaseSizes
{
    constexpr float small = 1.875;
    constexpr float mid = 2.0;
    constexpr float large = 2.125;
}

    struct Line
    {
        // .lines info
        int32_t brush_type;
        int32_t color;
        int32_t unknown_line_attribute;
        float brush_base_size;  // 1.875, 2.0, 2.125
        int32_t npoints;

        // meta
        std::list< Point > points;
    };
}
