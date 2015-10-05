/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef tile_h
#define tile_h

#include <cstdint>

class Tile{
public:

    enum class Type : std::uint8_t{
        WHEAT,
        TREE,
        BUILDING_UNFINISHED,
        BUILDING_CITY
    };

private:

    Type type;

public:

    Tile();

    //pixels
    std::int32_t get_size();
};

#endif
