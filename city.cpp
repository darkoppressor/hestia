/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "city.h"
#include "game_constants.h"
#include "game.h"

using namespace std;

City::City(){
    parent_civilization=0;

    breeding_counter=Game_Constants::BREEDING_RATE;
}

City::City(uint32_t new_parent){
    parent_civilization=new_parent;

    breeding_counter=Game_Constants::BREEDING_RATE;
}

uint32_t City::get_parent_civilization() const{
    return parent_civilization;
}

void City::set_parent_civilization(uint32_t new_parent){
    parent_civilization=new_parent;
}

Coords<uint32_t> City::get_tile() const{
    return tile;
}

void City::set_tile(const Coords<uint32_t>& new_tile){
    tile=new_tile;
}

const vector<uint32_t>& City::get_people() const{
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

int32_t City::get_size() const{
    return int32_t(Game_Constants::BUILDING_SIZE*Game_Constants::TILE_SIZE);
}

int32_t City::get_x() const{
    return int32_t(tile.x*Game_Constants::TILE_SIZE);
}

int32_t City::get_y() const{
    return int32_t(tile.y*Game_Constants::TILE_SIZE);
}

int32_t City::get_center_x() const{
    return int32_t(tile.x*Game_Constants::TILE_SIZE)+get_size()/2;
}

int32_t City::get_center_y() const{
    return int32_t(tile.y*Game_Constants::TILE_SIZE)+get_size()/2;
}

uint32_t City::get_chunk_x() const{
    return tile.x/Game_Constants::CHUNK_SIZE;
}

uint32_t City::get_chunk_y() const{
    return tile.y/Game_Constants::CHUNK_SIZE;
}

Collision_Rect<int32_t> City::get_spawn_zone() const{
    //pixels
    int32_t spawn_zone_x=get_center_x()-get_size()*4;
    int32_t spawn_zone_y=get_center_y()-get_size()*4;
    int32_t spawn_zone_width=get_size()*8;
    int32_t spawn_zone_height=get_size()*8;

    if(spawn_zone_x<0){
        spawn_zone_x=0;
    }
    if(spawn_zone_y<0){
        spawn_zone_y=0;
    }
    if(spawn_zone_x+spawn_zone_width+Game_Constants::PERSON_SIZE>=Game::get_world_width()){
        spawn_zone_width=Game::get_world_width()-1-spawn_zone_x-Game_Constants::PERSON_SIZE;
    }
    if(spawn_zone_y+spawn_zone_height+Game_Constants::PERSON_SIZE>=Game::get_world_height()){
        spawn_zone_height=Game::get_world_height()-1-spawn_zone_y-Game_Constants::PERSON_SIZE;
    }

    return Collision_Rect<int32_t>(spawn_zone_x,spawn_zone_y,spawn_zone_width,spawn_zone_height);
}

uint32_t City::get_population() const{
    return people.size();
}

void City::breed(uint32_t index,RNG& rng){
    if(get_population()<Game_Constants::CITY_POPULATION_MAX){
        if(--breeding_counter==0){
            breeding_counter=Game_Constants::BREEDING_RATE;

            Collision_Rect<int32_t> box_spawn_zone=get_spawn_zone();

            int32_t x=(int32_t)rng.random_range((uint32_t)box_spawn_zone.x,(uint32_t)(box_spawn_zone.x+box_spawn_zone.w));
            int32_t y=(int32_t)rng.random_range((uint32_t)box_spawn_zone.y,(uint32_t)(box_spawn_zone.y+box_spawn_zone.h));

            Game::new_people.push_back(Person(index,Collision_Rect<int32_t>(x,y,Game_Constants::PERSON_SIZE,Game_Constants::PERSON_SIZE)));
        }
    }
}
