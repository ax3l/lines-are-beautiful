#pragma once

#include "Line.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
    struct Layer
    {
        // .lines info
        int32_t nlines;
        
        // meta
        // current layer no.
        // is this layer visible
        std::list< Line > lines;
    };
}
