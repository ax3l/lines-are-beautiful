#pragma once

#include <cstdint>  // int8_t - int64_t
#include <string>
#include <fstream>
#include <list>

#include "Page.hpp"


namespace rmlab
{
    struct Notebook
    {
        // .lines info
        int32_t npages;
        
        // meta
        std::ifstream fstream;
        std::list< Page > pages;

        Notebook() = delete;
        Notebook( std::string const filename );
        ~Notebook();
    };
}
