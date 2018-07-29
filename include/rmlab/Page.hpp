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

#include "Layer.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
    struct Page
    {
        // .lines info
        int32_t nlayers;

        // meta
        // current page no.
        std::list< Layer > layers;
    };

    inline Page make_page( std::list< Layer > layers = std::list< Layer >{} )
    {
        Page newPage;
        newPage.nlayers = layers.size();
        newPage.layers = layers;

        return newPage;
    }
}
