/* Copyright 2017-2018 Axel Huebl, Matteo Delabre
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

#include <rmlab/rmlab.hpp>

#include <string>
#include <iostream>
#include <cmath>


int
main(
    int argc,
    char * argv[]
)
{
    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[0] << " path/to/filePrefix" << std::endl;
        return 1;
    }

    rmlab::Notebook notebook( argv[1] );

    if( notebook.pages.size() == 0 )
    {
        std::cerr << "File opening failed!" << std::endl;
        return 2;
    }

    std::cout << "no. of pages: " << notebook.pages.size() << '\n';
    std::size_t page_id = 0u;

    for( const auto & page : notebook.pages )
    {
        std::cout << "page " << page_id
                  << " -------------------------\n"
                  << "  no. of layers: " << page.layers.size() << '\n';

        std::size_t layer_id = 0u;

        for( const auto & layer : page.layers )
        {
            std::cout << "  layer " << layer_id
                      << " ----------------------\n"
                      << "    no. of lines: " << layer.lines.size() << '\n';

            std::size_t line_id = 0u;

            for( const auto & line : layer.lines )
            {
                std::cout << "    line " << line_id
                          << " ---------------------\n"
                          << "      brush type: " << line.brush_type << '\n'
                          << "      color int32: " << line.color << '\n'
                          << "      magic 4byte: "
                          << line.unknown_line_attribute << '\n'
                          << "      brush size: "
                          << line.brush_base_size << '\n'
                          << "      no. points: " << line.points.size() << '\n';

                std::size_t point_id = 0u;

                for( const auto & point : line.points )
                {
                    std::cout << "      point " << point_id
                              << " --------------------\n"
                              << "        coords: "
                              << point.x << ", " << point.y << '\n'
                              << "        speed: " << point.speed << '\n'
                              << "        direction: " << point.direction << '\n'
                              << "        width: " << point.width << '\n'
                              << "        pressure: " << point.pressure << '\n';

                    ++point_id;
                }

                ++line_id;
            }

            ++layer_id;
        }

        ++page_id;
    }

    return 0;
}
