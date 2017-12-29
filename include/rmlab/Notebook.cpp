#include "Notebook.hpp"

#include "Page.hpp"
#include "Layer.hpp"
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

#include "Line.hpp"
#include "Point.hpp"

#include <fstream>
#include <iostream>


namespace rmlab
{
namespace detail
{
    void
    readPoint(
        std::ifstream& fstream,
        Line& curLine
    )
    {
        Point curPoint;

        fstream.read( (char*)&curPoint.x, sizeof(float) );
        fstream.read( (char*)&curPoint.y, sizeof(float) );
        std::cout << "              coords: " << curPoint.x << ", "
                  << curPoint.y << std::endl;

        // pressure and rotation of the pen to page normal
        // rotation: for centrially symmetric brushes as now, one attribute
        //           would be sufficient,
        //           let's add a flat nib, calligraphic pen as conversion target! :)
        // range [0.0:1.0]
        fstream.read( (char*)&curPoint.pressure, sizeof(float) );
        // range [-pi/2:pi/2]
        fstream.read( (char*)&curPoint.rotX, sizeof(float) );
        // range [-pi/2:pi/2]
        fstream.read( (char*)&curPoint.rotY, sizeof(float) );
        std::cout << "            pressure: " << curPoint.pressure << std::endl;
        std::cout << "       rot to X axis: " << curPoint.rotX << std::endl;
        std::cout << "       rot to Y axis: " << curPoint.rotY << std::endl;

        curLine.points.emplace_back( curPoint );
    }

    void
    readLine(
        std::ifstream& fstream,
        Layer& curLayer
    )
    {
        Line curLine;

        // select 1-1: 2 (pen)
        // select 1-2: 3 (pen)
        // select 1-3: 4 (fine liner)
        // select 2-1: 7 (pencil sharp)
        // select 2-2: 1 (pencil wide)
        // select 3:   0 (brush)
        // select 4:   5 (marker/highlighter: always color 0)
        // what is/was 6? :-)
        fstream.read( (char*)&curLine.brush_type, sizeof(int32_t) );
        std::cout << "      brush type:  " << curLine.brush_type << std::endl;

        // color (0: black, 1: grey, 2: white)
        fstream.read( (char*)&curLine.color, sizeof(int32_t) );
        std::cout << "      color int32: " << curLine.color << std::endl;

        // unknown 4 Byte (int32_t?), always zero?
        // non-stored information about "selected" lines?
        fstream.read( (char*)&curLine.unknown_line_attribute, sizeof(int32_t) );
        std::cout << "      magic 4byte:  " << curLine.unknown_line_attribute << std::endl;

        // brush base size: 1.875, 2.0, 2.125
        fstream.read( (char*)&curLine.brush_base_size, sizeof(float) );
        std::cout << "      brush size:  " << curLine.brush_base_size << std::endl;

        fstream.read( (char*)&curLine.npoints, sizeof(int32_t) );
        std::cout << "      no. points: " << curLine.npoints << std::endl;
        
        for( int n = 0; n < curLine.npoints; ++n )
        {
            std::cout << "      point " << n << " --------------------" << std::endl;
            readPoint( fstream, curLine );
        }
        
        curLayer.lines.emplace_back( curLine );
    }

    void
    readLayer(
        std::ifstream& fstream,
        Page& curPage
    )
    {
        Layer curLayer;
        fstream.read( (char*)&curLayer.nlines, sizeof(int32_t) );
        std::cout << "    no.of lines: " << curLayer.nlines << std::endl;
        
        for( int nl = 0; nl < curLayer.nlines; ++nl )
        {
            std::cout << "    line " << nl << " ---------------------" << std::endl;
            readLine( fstream, curLayer );
        }
        
        curPage.layers.emplace_back( curLayer );
    }
}

    Notebook::Notebook( std::string const openFilename ) :
        npages( 0 ), filename( openFilename )
    {
        std::cout << "Opening file: " << filename + std::string( ".lines" ) << std::endl;
        std::ifstream fstream(
            filename + std::string( ".lines" ),
            std::ios::binary
        );
        
        // skip header
        fstream.seekg( 43, fstream.beg );
        
        fstream.read( (char*)&npages, sizeof(int32_t) );
        std::cout << "no.of pages: " << npages << std::endl;
        
        for( int p = 0; p < npages; ++p )
        {
            std::cout << "page " << p << " -------------------------" << std::endl;
            // layers
            Page curPage;
            fstream.read( (char*)&curPage.nlayers, sizeof(int32_t) );
            std::cout << "  no.o.layers: " << curPage.nlayers << std::endl;

            for( int nlay = 0; nlay < curPage.nlayers; ++nlay )
            {
                std::cout << "  layer " << nlay << " ----------------------" << std::endl;
                detail::readLayer( fstream, curPage );
            }

            pages.emplace_back( curPage );
        }
        
        fstream.close();
    }

    Notebook::~Notebook()
    {
    }
}
