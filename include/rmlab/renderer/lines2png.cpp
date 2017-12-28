#include <rmlab/rmlab.hpp>

#include <pngwriter.h>

#include <string>
#include <iostream>


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
    
    // draw png
    
    pngwriter png(
        1404,
        1872,
        65535,
        "test.png"
    );

    for( auto & page : myNotebook.pages )
    {
        for( auto & layer : page.layers )
        {
            for( auto & line : layer.lines )
            {
                float lx( 0. );
                float ly( 0. );
                for( auto & point : line.points )
                {
                    float dx = point.x - lx;
                    float dy = point.y - ly;

                    for( int x = int(point.x); x < int(point.x) + 20; ++x )
                    {
                        for( int y = int(point.y); y < int(point.y) + 50; ++y )
                        {
                            if( y - point.y > 6 and y - point.y < 30 )
                                continue;

                            png.plot(
                                x,
                                1872 - y,
                                // color
                                0.0, 0.0, 0.0
                                //dx / 2., dy / 2., 0.5
                            );
                        }
                    }
                    lx = point.x;
                    ly = point.y;
                }
            }
        }
    }
    
    png.close();

    return 0;
}
