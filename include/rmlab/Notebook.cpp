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
#include "rmlab/auxiliary/Filesystem.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>


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

        fstream.read( (char*)&curPoint.speed, sizeof(float) );
        fstream.read( (char*)&curPoint.direction, sizeof(float) );
        fstream.read( (char*)&curPoint.width, sizeof(float) );

        // pressure and rotation of the pen to page normal
        // rotation: for centrially symmetric brushes as now, one attribute
        //           would be sufficient,
        //           let's add a flat nib, calligraphic pen as conversion target! :)
        // range [0.0:1.0]
        fstream.read( (char*)&curPoint.pressure, sizeof(float) );

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

        // color (0: black, 1: grey, 2: white)
        fstream.read( (char*)&curLine.color, sizeof(int32_t) );

        // unknown 4 Byte (int32_t?), always zero?
        // non-stored information about "selected" lines?
        fstream.read( (char*)&curLine.unknown_line_attribute, sizeof(int32_t) );

        // brush base size: 1.875, 2.0, 2.125
        fstream.read( (char*)&curLine.brush_base_size, sizeof(float) );
        fstream.read( (char*)&curLine.npoints, sizeof(int32_t) );

        for( int n = 0; n < curLine.npoints; ++n )
        {
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

        for( int nl = 0; nl < curLayer.nlines; ++nl )
        {
            readLine( fstream, curLayer );
        }

        curPage.layers.emplace_back( curLayer );
    }
}

    Notebook::Notebook( std::string const openPathUUID ) :
        version(3), npages( 0 ), pathUUID( openPathUUID )
    {
        // append directory_separator if missing
        std::string const sep( &auxiliary::directory_separator, 1 );
        if( pathUUID.compare( pathUUID.size() - 1u, 1, sep) != 0 )
            pathUUID.append(sep);

        if( ! auxiliary::directory_exists( pathUUID ) )
        {
            std::cerr << "Path '" << pathUUID
                      << "' not found or not accessible!\n";
            return;
        }

        auto lsPath = auxiliary::list_directory( pathUUID );
        for( auto& lsEntry: lsPath )
        {
            if( lsEntry.compare( lsEntry.size() - 3u, 3u, ".rm") != 0 )
                continue;

            npages += 1;
        }

        if( npages == 0 )
        {
            std::cerr << "No pages found in '" << openPathUUID
                      << "'!\n";
            return;
        }

        for( int p = 0; p < npages; ++p )
        {
            std::string fullname = pathUUID + std::to_string(p) + std::string(".rm");
            std::ifstream fstream( fullname, std::ios::binary | std::ifstream::in );
            if( !fstream.good() )
            {
                std::cerr << "File '" << fullname
                          << "' not found or not accessible!\n";
                return;
            }

            // skip header
            fstream.seekg( 32, fstream.beg );

            // version
            char str_version;
            fstream.read( &str_version, sizeof(char) );
            version = std::atoi( &str_version );
            if( version != 3 )
                std::cerr << "WARNING: Unknown version!\n";

            // skip 10x space padding
            fstream.seekg( 10, fstream.cur );

            // layers
            Page curPage;
            fstream.read( (char*)&curPage.nlayers, sizeof(int32_t) );

            for( int nlay = 0; nlay < curPage.nlayers; ++nlay )
            {
                detail::readLayer( fstream, curPage );
            }

            fstream.close();

            pages.emplace_back( curPage );
        }

    }

    Notebook::Notebook() :
        npages( 0 )
    {
    }

    Notebook::~Notebook()
    {
    }

    void Notebook::save( std::string const path )
    {
        // append directory_separator if missing
        std::string rmPath = path;
        std::string const sep( &auxiliary::directory_separator, 1 );
        if( rmPath.compare( rmPath.size() - 1u, 1, sep) != 0 )
            rmPath.append(sep);

        bool pathReady = true;
        if( ! auxiliary::directory_exists( rmPath ) )
            pathReady = auxiliary::create_directories( rmPath );

        if( !pathReady )
        {
            std::cerr << "Path '" << path
                      << "' not accessible!\n";
            return;
        }

        int p = 0;
        for( auto & page : pages )
        {
            std::string fullname = rmPath + std::to_string(p) + std::string(".rm");
            std::ofstream fstream(
                fullname,
                std::fstream::out | std::ios::binary
            );

            if( !fstream.good() )
            {
                std::cerr << "File '" << fullname
                          << "' not accessible!\n";
                return;
            }

            // write header (33 bytes)
            fstream.write( "reMarkable .lines file, version=3", 33 );

            // write space padding
            fstream.write( "          ", 10 );

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

                        fstream.write( (char*)&point.speed, sizeof(float) );
                        fstream.write( (char*)&point.direction, sizeof(float) );
                        fstream.write( (char*)&point.width, sizeof(float) );
                        fstream.write( (char*)&point.pressure, sizeof(float) );
                    }
                }
            }

            fstream.close();
            p++;
        }
    }
}

