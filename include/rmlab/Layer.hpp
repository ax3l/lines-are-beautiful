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
 * Definition of layers.
 */

#pragma once

#include "Line.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
    /**
     * Element of a page, containing a set of lines.
     *
     * When inside the same layer, a line A is rendered above another line B
     * if and only if it A comes after B in the layer order.
     *
     * @see rmlab::Page
     * @see rmlab::Line
     */
    struct Layer
    {
        /**
         * Number of lines in this layer.
         */
        int32_t nlines;

        /**
         * All lines contained in this layer, in the order they were drawn.
         */
        std::list< Line > lines;
    };
}
