/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef game_selection_h
#define game_selection_h

#include <collision/coords.h>

#include <cstdint>

class Game_Selection {
    public:
        enum class Type : std::uint8_t {
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

        Game_Selection ();

        void clear_type();
};

#endif
