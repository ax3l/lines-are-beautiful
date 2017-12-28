#pragma once

#include "Layer.hpp"

#include <cstdint>  // int8_t - int64_t
#include <list>


namespace rmlab
{
    struct Page
    {
        // .lines info
        int32_t nlayers;

        // meta
        // current page no.
        std::list< Layer > layers;
    };
}
