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

bool AI_Goal::is_gather_wheat() const{
    return type==Type::GATHER_WHEAT;
}

bool AI_Goal::is_gather_tree() const{
    return type==Type::GATHER_TREE;
}

bool AI_Goal::target_is_person() const{
    return is_attack_person_melee();
}

bool AI_Goal::is_gather() const{
    return is_gather_wheat() || is_gather_tree();
}

bool AI_Goal::is_empty_inventory() const{
    return type==Type::EMPTY_INVENTORY;
}

bool AI_Goal::is_eat() const{
    return type==Type::EAT;
}

bool AI_Goal::is_eat_at_home() const{
    return type==Type::EAT_AT_HOME;
}

bool AI_Goal::is_forage() const{
    return type==Type::FORAGE;
}

bool AI_Goal::is_retreat() const{
    return type==Type::RETREAT;
}

bool AI_Goal::is_attack_person_melee() const{
    return type==Type::ATTACK_PERSON_MELEE;
}

Tile::Type AI_Goal::get_goal_tile_type() const{
    if(type==Type::GATHER_WHEAT){
        return Tile::Type::WHEAT;
    }
    else if(type==Type::GATHER_TREE){
        return Tile::Type::TREE;
    }
    else if(type==Type::FORAGE){
        return Tile::Type::WHEAT;
    }
    else{
        return Tile::Type::WHEAT;
    }
}

void AI_Goal::set_type(Type new_type){
    type=new_type;

    counter=get_counter_max();
}

void AI_Goal::clear_goal(){
    set_type(Type::NONE);

    coords.x=0;
    coords.y=0;
}

Coords<uint32_t> AI_Goal::get_coords_tiles() const{
    return coords;
}

Coords<int32_t> AI_Goal::get_coords_pixels() const{
    return Coords<int32_t>((int32_t)coords.x,(int32_t)coords.y);
}

uint32_t AI_Goal::get_person_index() const{
    return coords.x;
}

void AI_Goal::set_coords_tiles(const Coords<uint32_t>& new_coords){
    coords=new_coords;
}

void AI_Goal::set_coords_pixels(const Coords<int32_t>& new_coords){
    coords.x=(uint32_t)new_coords.x;
    coords.y=(uint32_t)new_coords.y;
}

void AI_Goal::set_person_index(uint32_t new_index){
    coords.x=new_index;
    coords.y=0;
}

uint32_t AI_Goal::get_counter_max() const{
    if(is_gather()){
        return (Game_Constants::GATHER_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_empty_inventory()){
        return (Game_Constants::EMPTY_INVENTORY_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_eat()){
        return (Game_Constants::EAT_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_eat_at_home()){
        return (Game_Constants::EAT_AT_HOME_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_forage()){
        return (Game_Constants::FORAGE_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_retreat()){
        return (Game_Constants::RETREAT_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_attack_person_melee()){
        return (Game_Constants::ATTACK_PERSON_MELEE_RATE*Engine::UPDATE_RATE)/1000;
    }
    else{
        return 0;
    }
}

bool AI_Goal::is_first_count() const{
    return counter==get_counter_max();
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

AI_Choice::AI_Choice(AI_Goal::Type new_goal_type,int32_t new_priority){
    goal_type=new_goal_type;

    priority=new_priority;
}

bool AI_Choice::operator<=(const AI_Choice& choice) const{
    return priority<=choice.priority;
}
