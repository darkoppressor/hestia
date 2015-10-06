/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef chunk_h
#define chunk_h

#include "region.h"

#include <string>
#include <cstdint>

class Chunk{
private:

    std::uint32_t parent_region;

public:

    Chunk();

    Region* get_parent_region();
    void set_parent_region(std::uint32_t new_parent);

    std::string get_ground_string();

    //pixels
    std::int32_t get_size();

    //pixels
    static std::int32_t get_x(std::uint32_t chunk_x);
    static std::int32_t get_y(std::uint32_t chunk_y);

    void render_ground(std::uint32_t chunk_x,std::uint32_t chunk_y);
};

#endif
