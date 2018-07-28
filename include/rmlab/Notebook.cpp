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

#include "Notebook.hpp"
#include "Page.hpp"
#include "Layer.hpp"
#include "Line.hpp"
#include "Point.hpp"

#include <fstream>
#include <iostream>

#ifdef RMLAB_ENABLE_LOGGING
#define LOG(arg) (std::cerr << arg)
#else
#define LOG(arg)
#endif


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
        LOG( "              coords: " << curPoint.x << ", "
                << curPoint.y << '\n' );

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
        LOG( "            pressure: " << curPoint.pressure << '\n' );
        LOG( "       rot to X axis: " << curPoint.rotX << '\n' );
        LOG( "       rot to Y axis: " << curPoint.rotY << '\n' );

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
        LOG( "      brush type:  " << curLine.brush_type << '\n' );

        // color (0: black, 1: grey, 2: white)
        fstream.read( (char*)&curLine.color, sizeof(int32_t) );
        LOG( "      color int32: " << curLine.color << '\n' );

        // unknown 4 Byte (int32_t?), always zero?
        // non-stored information about "selected" lines?
        fstream.read( (char*)&curLine.unknown_line_attribute, sizeof(int32_t) );
        LOG( "      magic 4byte:  " << curLine.unknown_line_attribute << '\n' );

        // brush base size: 1.875, 2.0, 2.125
        fstream.read( (char*)&curLine.brush_base_size, sizeof(float) );
        LOG( "      brush size:  " << curLine.brush_base_size << '\n' );

        fstream.read( (char*)&curLine.npoints, sizeof(int32_t) );
        LOG( "      no. points: " << curLine.npoints << '\n' );

        for( int n = 0; n < curLine.npoints; ++n )
        {
            LOG( "      point " << n << " --------------------\n" );
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
        LOG( "    no.of lines: " << curLayer.nlines << '\n' );

        for( int nl = 0; nl < curLayer.nlines; ++nl )
        {
            LOG( "    line " << nl << " ---------------------\n" );
            readLine( fstream, curLayer );
        }

        curPage.layers.emplace_back( curLayer );
    }
}

    Notebook::Notebook( std::string const openFilename ) :
        npages( 0 ), filename( openFilename )
    {
        std::string fullname = filename + ".lines";

        LOG( "Opening file: " << fullname << '\n' );
        std::ifstream fstream( fullname, std::ios::binary );

        // skip header
        fstream.seekg( 43, fstream.beg );

        fstream.read( (char*)&npages, sizeof(int32_t) );
        LOG( "no. of pages: " << npages << '\n' );

        for( int p = 0; p < npages; ++p )
        {
            LOG( "page " << p << " -------------------------\n" );
            // layers
            Page curPage;
            fstream.read( (char*)&curPage.nlayers, sizeof(int32_t) );
            LOG( "  no. of layers: " << curPage.nlayers << '\n' );

            for( int nlay = 0; nlay < curPage.nlayers; ++nlay )
            {
                LOG( "  layer " << nlay << " ----------------------\n" );
                detail::readLayer( fstream, curPage );
            }

            pages.emplace_back( curPage );
        }

        fstream.close();
    }

    Notebook::Notebook() :
        npages( 0 )
    {
    }

    Notebook::~Notebook()
    {
    }

    void Notebook::save( std::string const saveFilename )
    {
        std::cout << "Saving to file: " << saveFilename + std::string( ".lines" )
                  << std::endl;
        std::ofstream fstream(
            saveFilename + std::string( ".lines" ),
            std::fstream::out | std::ios::binary
        );

        // write header (43 bytes)
        fstream.write( "reMarkable lines with selections and layers", 43 );

        // pages
        npages = pages.size();
        fstream.write(
            (char*)&npages,
            sizeof(int32_t)
        );

        for( auto & page : pages )
        {
            // layers
            int32_t nlayers = page.layers.size();
            fstream.write( (char*)&nlayers, sizeof(int32_t) );
            for( auto & layer : page.layers )
            {
                // lines
                int32_t nlines = layer.lines.size();
                fstream.write( (char*)&nlines, sizeof(int32_t) );
                for( auto & line : layer.lines )
                {
                    int32_t brush_type = line.brush_type;
                    fstream.write( (char*)&brush_type, sizeof(int32_t) );

                    int32_t color = line.color;
                    fstream.write( (char*)&color, sizeof(int32_t) );

                    int32_t unknown_line_attribute = line.unknown_line_attribute;
                    fstream.write( (char*)&unknown_line_attribute, sizeof(int32_t) );

                    float brush_base_size = line.brush_base_size;
                    fstream.write( (char*)&brush_base_size, sizeof(float) );

                    // points
                    int32_t npoints = line.points.size();
                    fstream.write( (char*)&npoints, sizeof(int32_t) );
                    for( auto & point : line.points )
                    {
                        fstream.write( (char*)&point.x, sizeof(float) );
                        fstream.write( (char*)&point.y, sizeof(float) );

                        fstream.write( (char*)&point.pressure, sizeof(float) );
                        fstream.write( (char*)&point.rotX, sizeof(float) );
                        fstream.write( (char*)&point.rotY, sizeof(float) );
                    }
                }
            }
        }

        fstream.close();
    }
}
