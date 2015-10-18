/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_selection.h"

using namespace std;

Game_Selection::Game_Selection(){
    type=Type::NONE;

    index=0;

    follow=false;
}

void Game_Selection::clear_type(){
    type=Type::NONE;

    index=0;

    follow=false;
}
