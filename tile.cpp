/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "tile.h"
#include "game_constants.h"

using namespace std;

Tile::Tile(){
    type=Type::WHEAT;
}

int32_t Tile::get_size(){
    if(type==Type::BUILDING_UNFINISHED || type==Type::BUILDING_CITY){
        return (int32_t)(Game_Constants::BUILDING_SIZE*Game_Constants::TILE_SIZE);
    }
    else{
        return (int32_t)Game_Constants::TILE_SIZE;
    }
}
