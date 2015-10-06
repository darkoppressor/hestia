/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "city.h"
#include "game_constants.h"

using namespace std;

/**Tile* City::get_tile_ptr(){
    if(Game::tile_exists(tile)){
        return &Game::tiles[tile];
    }
    else{
        return 0;
    }
}*/

City::City(){
    parent_civilization=0;
}

City::City(uint32_t new_parent){
    parent_civilization=new_parent;
}

uint32_t City::get_parent_civilization(){
    return parent_civilization;
}

void City::set_parent_civilization(uint32_t new_parent){
    parent_civilization=new_parent;
}

Coords<uint32_t> City::get_tile(){
    return tile;
}

void City::set_tile(const Coords<uint32_t>& new_tile){
    tile=new_tile;
}

const vector<uint32_t>& City::get_people(){
    return people;
}

void City::add_person(uint32_t person){
    for(size_t i=0;i<people.size();i++){
        if(people[i]==person){
            //Exit early, because this person is already in the list
            return;
        }
    }

    people.push_back(person);
}

void City::remove_person(uint32_t person){
    for(size_t i=0;i<people.size();i++){
        if(people[i]==person){
            people.erase(people.begin()+i);

            break;
        }
    }
}

int32_t City::get_size(){
    return int32_t(Game_Constants::BUILDING_SIZE*Game_Constants::TILE_SIZE);
}

int32_t City::get_x(){
    return int32_t(tile.x*Game_Constants::TILE_SIZE);
}

int32_t City::get_y(){
    return int32_t(tile.y*Game_Constants::TILE_SIZE);
}

int32_t City::get_center_x(){
    return int32_t(tile.x*Game_Constants::TILE_SIZE)+get_size()/2;
}

int32_t City::get_center_y(){
    return int32_t(tile.y*Game_Constants::TILE_SIZE)+get_size()/2;
}
