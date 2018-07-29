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
 * Definition of pages.
 */

#pragma once

#include "Layer.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
    /**
     * Element of a notebook, containing a stack of layers.
     *
     * The layers at the top of the stack (or at the end of the list) are
     * rendered last, meaning that they will appear above others.
     *
     * @see rmlab::Notebook
     * @see rmlab::Layer
     */
    struct Page
    {
        /**
         * Number of layers in this page.
         */
        int32_t nlayers;

        /**
         * All the layers of this page, from the bottom to the top of the
         * layer stack.
         */
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
