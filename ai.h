/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ai_h
#define ai_h

#include "tile.h"

#include <coords.h>

#include <cstdint>

class AI_Goal{
public:

    enum class Type : std::uint8_t{
        NONE,
        GATHER_WHEAT,
        GATHER_TREE,
        EMPTY_INVENTORY
    };

    Type type;

    //These represent different kinds of coordinates depending on type
    Coords<std::uint32_t> coords;

    std::uint32_t counter;

    AI_Goal();

    bool is_in_progress() const;
    bool is_first_count() const;
    bool is_gather() const;
    bool is_empty_inventory() const;

    Tile::Type get_gather_tile_type() const;

    void set_type(Type new_type);

    //Returns true if the countdown completed,
    //or false if the countdown did not complete
    bool countdown();
};

#endif
