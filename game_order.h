/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_order_h
#define game_order_h

#include "tile.h"

#include <coords.h>

#include <cstdint>

class Game_Order{
public:

    enum class Type : std::uint8_t{
        NONE,
        BUILD_CITY,
        REPOPULATE_CITY,
        ABANDON_CITY,
        CANCEL_UNFINISHED_BUILDING
    };

    Type type;

    //These represent different kinds of coordinates (or other data) depending on type
    //BUILD_CITY: the tile coordinates of the new unfinished building tile
    //REPOPULATE_CITY: x represents the city's index, y is unused
    //ABANDON_CITY: x represents the city's index, y is unused
    //CANCEL_UNFINISHED_BUILDING: the tile coordinates of the unfinished building tile to cancel
    Coords<std::uint32_t> coords;

    std::uint32_t leader;

    Game_Order(Type new_type,const Coords<std::uint32_t>& new_coords,std::uint32_t new_leader);

    Tile::Type get_tile_type() const;
    Coords<std::int32_t> get_pixel_coords() const;
    bool is_valid() const;

    void execute();

    void render() const;
};

#endif
