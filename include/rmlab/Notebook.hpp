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
 * Definition of notebooks.
 */

#pragma once

#include <cstdint>  // int8_t - int64_t
#include <string>
#include <list>

#include "Page.hpp"


namespace rmlab
{
    /**
     * Base unit of the reMarkable note-taking system, used similarly
     * to a real-world notebook. It is made up of a set of pages.
     *
     * @see rmlab::Page
     */
    struct Notebook
    {
        /**
         * Version of the file format in this notebook.
         */
        int32_t version;

        /**
         * Number of pages in this notebook.
         */
        int32_t npages;

        /**
         * Path ending in a uuid directory containing the notebook
         */
        std::string pathUUID;

        /**
         * All the pages of this notebook.
         */
        std::list< Page > pages;

        /**
         * Open a new, empty notebook.
         */
        Notebook();

        /**
         * Open a notebook.
         *
         * @param openPathUUID path to the UUID-named directory containing the notebook.
         */
        Notebook( std::string const openPathUUID );
        ~Notebook();

        void save( std::string const path );
    };
}
