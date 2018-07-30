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

#include <rmlab/rmlab.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>
#include <memory>


/**
 * Generate a SVG path data representation of a list of points.
 *
 * @param points Sequence of points to link together.
 * @param out Stream into which data should be outputted.
 */
void render_path_data(
    const std::list< rmlab::Point > & points,
    std::ostream & out
)
{
    bool is_first = true;

    for( const auto & point : points )
    {
        out << (is_first ? "M" : "L");
        out << point.x << "," << point.y;

        is_first = false;
    }
}

/**
 * Generate a SVG path representation of an erased area resulting
 * from the use of the rubber area tool. Note that this representation
 * is intended to be used as a mask; because of that, it will appear
 * black if rendered outside a mask.
 *
 * @param line Line enclosing the area.
 * @param out Stream into which data should be outputted.
 */
void render_rubber_area( const rmlab::Line & line, std::ostream & out )
{
    out << "<path ";
    out << "fill=\"black\" ";
    out << "stroke=\"none\" ";
    out << "d=\"";
    render_path_data( line.points, out );
    out << "\" />";
}

/**
 * Generate a SVG path representation of a line drawn using one of
 * the tools that produce lines. Lines drawn using the rubber tool
 * are rendered black because they are intended to be used inside
 * a mask.
 *
 * @param line Line to be rendered.
 * @param out Stream into which data should be outputted.
 */
void render_normal_line( const rmlab::Line & line, std::ostream & out )
{
    // TODO: apply brush texture, pressure and tilt parameters

    out << "<path ";
    out << "fill=\"none\" ";
    out << "stroke=\"";

    switch( line.color )
    {
    case rmlab::Colors::grey:
        out << "grey";
        break;

    case rmlab::Colors::white:
        out << "white";
        break;

    default:
        out << "black";
        break;
    }

    out << "\" ";
    out << "stroke-width=\"";

    switch( line.brush_type )
    {
    case rmlab::Brushes::highlighter:
    case rmlab::Brushes::rubber:
        out << 20 * line.brush_base_size;
        break;

    default:
        out << 18 * line.brush_base_size - 32;
        break;
    }

    out << "\" ";

    out << "d=\"";
    render_path_data( line.points, out );
    out << "\" ";

    if( line.brush_type == rmlab::Brushes::highlighter )
    {
        out << "opacity=\"0.25\" ";
    }

    out << "stroke-linejoin=\"round\" ";
    out << "stroke-linecap=\"round\" ";
    out << "/>";
}

/**
 * Generate a SVG path representation of any line.
 *
 * @param line Line to be rendered.
 * @param out Stream into which data should be outputted.
 */
void render_line( const rmlab::Line & line, std::ostream & out )
{
    switch( line.brush_type )
    {
    case rmlab::Brushes::rubber_area:
        render_rubber_area( line, out );
        break;

    default:
        render_normal_line( line, out );
        break;
    }
}

/**
 * Generate a SVG group containing the representation of a layer.
 *
 * @param id Numeric unique identifier for this layer.
 * @param layer Layer to be rendered.
 * @param out Stream into which data should be outputted.
 */
void render_layer(
    std::size_t layer_id,
    const rmlab::Layer & layer,
    std::ostream & out )
{
    auto layer_id_str = "layer-" + std::to_string( layer_id );

    // The most difficult things to render in SVG are erasures:
    // we must ensure that they only apply to lines that were drawn
    // before them, and that they do not leak on other layers.

    // Given the following sample list of lines:
    //
    // * normal stroke #1
    // * normal stroke #2
    // * erasure #1
    // * normal stroke #3
    // * erasure #2
    // * normal stroke #4
    //
    // We aim at constructing the following group structure:
    //
    // Layer group
    // +- Subgroup masked by erasure #2
    // |  +- Subgroup masked by erasure #1
    // |  |  +- normal stroke #1
    // |  |  +- normal stroke #2
    // |  +- normal stroke #3
    // +- normal stroke #4

    struct RenderGroup
    {
        std::vector< rmlab::Line > strokes;
        std::vector< rmlab::Line > erasures;
    };

    // (First pass.) Cluster the list of lines and erasures into a
    // stack of groups so that a given group is only affected by
    // the erasures it contains and the ones above it.
    std::stack< RenderGroup > open_groups;
    RenderGroup current;

    for( const auto & line : layer.lines )
    {
        switch( line.brush_type )
        {
        case rmlab::Brushes::rubber:
        case rmlab::Brushes::rubber_area:
            if( !current.strokes.empty() )
            {
                // Register an erasure only if there exists previous
                // strokes to erase (otherwise, there is nothing to
                // erase and it can be safely ignored).
                current.erasures.push_back( line );
            }
            break;

        default:
            if( !current.erasures.empty() )
            {
                // We cannot add a new stroke to a group that already contains
                // some erasures, because this new stroke should not be
                // affected by the previous erasures; so, we create a new group.
                open_groups.push( std::move( current ) );
                current = RenderGroup{};
            }

            current.strokes.push_back( line );
            break;
        }
    }

    open_groups.push( std::move( current ) );

    // (Second pass.) Pop out groups from the stack, thereby creating
    // masks and opening the SVG groups. Prepare a reversed stack to
    // later close the groups.
    std::stack< RenderGroup > close_groups;
    std::size_t mask_id = 0;

    while( !open_groups.empty() )
    {
        // Create a mask group for all erasures of the group, if any
        if( !open_groups.top().erasures.empty() )
        {
            auto mask_id_str = layer_id_str + "-mask-"
                + std::to_string(mask_id);

            out << "<mask id=\"" << mask_id_str << "\">";
            out << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />";

            for( const auto & erasure : open_groups.top().erasures )
            {
                render_line( erasure, out );
            }

            out << "</mask>";
            out << "<g mask=\"url(#" << mask_id_str << ")\" ";
        }
        else
        {
            out << "<g ";
        }

        if( mask_id == 0 )
        {
            out << "id=\"" << layer_id_str << "\" ";
        }

        out << ">";

        close_groups.push( std::move( open_groups.top() ) );
        open_groups.pop();

        ++mask_id;
    }

    // (Third pass.) Generate paths for each stroke of each group and
    // then close the groups.
    while( !close_groups.empty() )
    {
        for( const auto & stroke : close_groups.top().strokes )
        {
            render_line( stroke, out );
        }

        out << "</g>";
        close_groups.pop();
    }
}

void render_page( const rmlab::Page& page, std::ostream& out )
{
    // SVG header
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";
    out << "<svg width=\"1404\" height=\"1872\" viewBox=\"0 0 1404 1872\" ";
    out << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" ";
    out << "xmlns:xlink=\"http://www.w3.org/1999/xlink\">";

    std::size_t layer_id = 0;

    for( const auto & layer : page.layers )
    {
        render_layer( layer_id, layer, out );
        ++layer_id;
    }

    // SVG footer
    out << "</svg>";
}

int
main(
    int argc,
    char * argv[]
)
{
    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[0] << " path/to/filePrefix" << std::endl;
        return 1.0;
    }

    rmlab::Notebook myNotebook( argv[1] );
    std::size_t page_id = 0;

    for( const auto & page : myNotebook.pages )
    {
        std::ostringstream page_filename;
        page_filename << "test-" << page_id << ".svg";
        std::ofstream page_file{ page_filename.str() };

        render_page( page, page_file );
        ++page_id;
    }

    return 0;
}
