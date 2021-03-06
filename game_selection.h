/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_selection_h
#define game_selection_h

#include <coords.h>

#include <cstdint>

class Game_Selection{
public:

    enum class Type : std::uint8_t{
        NONE,
        PERSON,
        CITY,
        UNFINISHED_BUILDING,
        CIVILIZATION
    };

    Type type;

    std::uint32_t index;

    Coords<std::uint32_t> tile_coordinates;

    bool follow;

    Game_Selection();

    void clear_type();
};

#endif
