/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ai.h"
#include "game_constants.h"

#include <engine.h>

using namespace std;

AI_Goal::AI_Goal(){
    type=Type::NONE;

    counter=0;
}

bool AI_Goal::is_in_progress() const{
    return type!=Type::NONE;
}

bool AI_Goal::is_first_count() const{
    if(is_gather()){
        return counter==Game_Constants::GATHER_RATE*Engine::UPDATE_RATE;
    }
    else if(is_empty_inventory()){
        return counter==Game_Constants::EMPTY_INVENTORY_RATE*Engine::UPDATE_RATE;
    }
    else{
        return false;
    }
}

bool AI_Goal::is_gather() const{
    return type==Type::GATHER_WHEAT || type==Type::GATHER_TREE;
}

bool AI_Goal::is_empty_inventory() const{
    return type==Type::EMPTY_INVENTORY;
}

Tile::Type AI_Goal::get_gather_tile_type() const{
    if(type==Type::GATHER_WHEAT){
        return Tile::Type::WHEAT;
    }
    else if(type==Type::GATHER_TREE){
        return Tile::Type::TREE;
    }
    else{
        return Tile::Type::WHEAT;
    }
}

void AI_Goal::set_type(Type new_type){
    type=new_type;

    if(is_gather()){
        counter=Game_Constants::GATHER_RATE*Engine::UPDATE_RATE;
    }
    else if(is_empty_inventory()){
        counter=Game_Constants::EMPTY_INVENTORY_RATE*Engine::UPDATE_RATE;
    }
    else{
        counter=0;
    }
}

bool AI_Goal::countdown(){
    if(is_in_progress()){
        //We check for counter being 0 first, because it is possible for a goal to have a countdown of 0 (i.e., instant)
        if(counter==0 || --counter==0){
            return true;
        }
    }

    return false;
}
