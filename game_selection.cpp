/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "game_selection.h"

using namespace std;

Game_Selection::Game_Selection () {
    type = Type::NONE;

    index = 0;

    follow = false;
}

void Game_Selection::clear_type () {
    type = Type::NONE;

    index = 0;

    tile_coordinates.x = 0;
    tile_coordinates.y = 0;

    follow = false;
}
