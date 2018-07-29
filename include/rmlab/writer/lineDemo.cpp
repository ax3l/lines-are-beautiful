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
        std::cerr << "Usage: " << argv[0]
                  << " path/to/outputFilePrefix"
                  << std::endl;
        return 1;
    }

    // rmlab::Notebook myNotebook( argv[1] );
    rmlab::Notebook myNotebook{};

    // page 1
    auto page1 = rmlab::make_page( { rmlab::make_layer( ) } );
    // add lines with one point
    auto line1 = rmlab::make_line( rmlab::Brushes::pencil_tilt,
        rmlab::Colors::black, 0, 2.125 );
    auto line2 = rmlab::make_line( rmlab::Brushes::pencil_tilt,
        rmlab::Colors::black, 0, 2.125 );
    auto line3 = rmlab::make_line( rmlab::Brushes::pencil_tilt,
        rmlab::Colors::black, 0, 2.125 );
    // note: spacing of 500 and above is not drawn
    for( int x = 200; x < 1200; x+=100 )
    {
        auto point = rmlab::make_point(
            float(x), // 0:1404
            200.f,    // 0:1872
            1.0f,     // pressure, range [0.0:1.0]
            M_PI / 2.0,     // rotX, range [-pi/2:pi/2]
            M_PI / 2.0      // rotY range [-pi/2:pi/2]
        );
        line1.points.emplace_back( point );
    }
    for( int x = 200; x < 1200; x+=200 )
    {
        auto point2 = rmlab::make_point(
            float(x), // 0:1404
            300.f,    // 0:1872
            1.0f,     // pressure, range [0.0:1.0]
            M_PI / 2.0,     // rotX, range [-pi/2:pi/2]
            M_PI / 2.0      // rotY range [-pi/2:pi/2]
        );
        line2.points.emplace_back( point2 );
    }
    for( int x = 200; x < 1200; x+=400 )
    {
        auto point3 = rmlab::make_point(
            float(x), // 0:1404
            400.f,    // 0:1872
            1.0f,     // pressure, range [0.0:1.0]
            M_PI / 2.0,     // rotX, range [-pi/2:pi/2]
            M_PI / 2.0      // rotY range [-pi/2:pi/2]
        );
        line3.points.emplace_back( point3 );
    }
    page1.layers.front().lines.emplace_back( line1 );
    page1.layers.front().lines.emplace_back( line2 );
    page1.layers.front().lines.emplace_back( line3 );
    myNotebook.pages.emplace_back( page1 );

    // save
    myNotebook.save( argv[1] );

    return 0;
}
