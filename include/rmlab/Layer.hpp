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

#include "Line.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
    struct Layer
    {
        // .lines info
        int32_t nlines;
        
        // meta
        // current layer no.
        // is this layer visible
        std::list< Line > lines;
    };

    inline Layer make_layer( std::list< Line > lines = std::list< Line >{} )
    {
        Layer newLayer;
        newLayer.nlines = lines.size();
        newLayer.lines = lines;

        return newLayer;
    }
}
