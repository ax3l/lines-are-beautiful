/* Copyright 2017 Axel Huebl
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

#include <cstdint>  // int8_t - int64_t
#include <string>
#include <fstream>
#include <list>

#include "Page.hpp"


namespace rmlab
{
    struct Notebook
    {
        // .lines info
        int32_t npages;
        
        // meta
        std::ifstream fstream;
        std::list< Page > pages;

        Notebook() = delete;
        Notebook( std::string const filename );
        ~Notebook();
    };
}
