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

string AI_Goal::get_type_string() const{
    if(is_gather_wheat()){
        return "Gather wheat";
    }
    else if(is_gather_tree()){
        return "Gather wood";
    }
    else if(is_empty_inventory()){
        return "Empty inventory";
    }
    else if(is_eat()){
        return "Eat";
    }
    else if(is_eat_at_home()){
        return "Return home to eat";
    }
    else if(is_forage()){
        return "Forage";
    }
    else if(is_retreat()){
        return "Retreat";
    }
    else if(is_attack_person_melee()){
        return "Attack person with melee";
    }
    else if(is_attack_building_melee()){
        return "Attack building with melee";
    }
    else if(is_build()){
        return "Build new city";
    }
    else if(is_repair()){
        return "Repair home city";
    }
    else{
        return "None";
    }
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

bool AI_Goal::is_combat() const{
    return is_retreat() || is_attack_person_melee() || is_attack_building_melee();
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

bool AI_Goal::is_attack_building_melee() const{
    return type==Type::ATTACK_BUILDING_MELEE;
}

bool AI_Goal::is_build() const{
    return type==Type::BUILD;
}

bool AI_Goal::is_repair() const{
    return type==Type::REPAIR;
}

bool AI_Goal::gather_can_interrupt() const{
    if(!is_in_progress()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::empty_inventory_can_interrupt() const{
    if(!is_in_progress() || is_gather()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::eat_can_interrupt() const{
    if(!is_in_progress() || is_gather() || is_empty_inventory() || is_eat_at_home() || is_forage() || is_retreat() || is_attack_person_melee() || is_attack_building_melee() ||
       is_build() || is_repair()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::eat_at_home_can_interrupt() const{
    if(!is_in_progress() || is_gather() || is_empty_inventory() || is_retreat() || is_attack_person_melee() || is_attack_building_melee() || is_build() ||
       is_repair()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::forage_can_interrupt() const{
    if(!is_in_progress() || is_gather() || is_empty_inventory() || is_eat_at_home() || is_retreat() || is_attack_person_melee() || is_attack_building_melee() ||
       is_build() || is_repair()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::retreat_can_interrupt() const{
    if(!is_in_progress() || is_gather() || is_forage() || is_attack_person_melee() || is_attack_building_melee() || is_build()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::attack_person_melee_can_interrupt() const{
    if(!is_in_progress() || is_gather() || is_empty_inventory() || is_retreat()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::attack_building_melee_can_interrupt() const{
    if(!is_in_progress() || is_gather() || is_empty_inventory() || is_retreat()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::build_can_interrupt() const{
    if(!is_in_progress() || is_empty_inventory()){
        return true;
    }
    else{
        return false;
    }
}

bool AI_Goal::repair_can_interrupt() const{
    if(!is_in_progress() || is_retreat()){
        return true;
    }
    else{
        return false;
    }
}

Tile::Type AI_Goal::get_goal_tile_type() const{
    if(type==Type::GATHER_WHEAT){
        return Tile::Type::WHEAT;
    }
    else if(type==Type::GATHER_TREE){
        return Tile::Type::TREE;
    }
    else if(type==Type::EMPTY_INVENTORY){
        return Tile::Type::BUILDING_CITY;
    }
    else if(type==Type::EAT_AT_HOME){
        return Tile::Type::BUILDING_CITY;
    }
    else if(type==Type::FORAGE){
        return Tile::Type::WHEAT;
    }
    else if(type==Type::ATTACK_BUILDING_MELEE){
        //When attacking a building, the goal tile type could actually be either a city building or an unfinished building
        //This function isn't used anywhere where this distinction is an issue, though
        return Tile::Type::BUILDING_CITY;
    }
    else if(type==Type::BUILD){
        return Tile::Type::BUILDING_UNFINISHED;
    }
    else if(type==Type::REPAIR){
        return Tile::Type::BUILDING_CITY;
    }
    else{
        return Tile::Type::WHEAT;
    }
}

void AI_Goal::reset_counter(){
    counter=get_counter_max();
}

void AI_Goal::set_type(Type new_type){
    type=new_type;

    reset_counter();
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
    else if(is_attack_building_melee()){
        return (Game_Constants::ATTACK_BUILDING_MELEE_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_build()){
        return (Game_Constants::BUILD_RATE*Engine::UPDATE_RATE)/1000;
    }
    else if(is_repair()){
        return (Game_Constants::REPAIR_RATE*Engine::UPDATE_RATE)/1000;
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
