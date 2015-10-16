/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "civilization.h"
#include "game.h"

#include <engine_strings.h>

using namespace std;

Civilization::Civilization(){
    parent_leader=0;
}

Civilization::Civilization(uint32_t new_parent){
    parent_leader=new_parent;
}

uint32_t Civilization::get_parent_leader() const{
    return parent_leader;
}

void Civilization::set_parent_leader(uint32_t new_parent){
    parent_leader=new_parent;
}

vector<uint32_t> Civilization::get_cities() const{
    return cities;
}

void Civilization::add_city(uint32_t city){
    for(size_t i=0;i<cities.size();i++){
        if(cities[i]==city){
            //Exit early, because this city is already in the list
            return;
        }
    }

    cities.push_back(city);
}

void Civilization::remove_city(uint32_t city){
    for(size_t i=0;i<cities.size();i++){
        if(cities[i]==city){
            cities.erase(cities.begin()+i);

            break;
        }
    }
}

vector<Coords<uint32_t>> Civilization::get_unfinished_buildings() const{
    return unfinished_buildings;
}

void Civilization::add_unfinished_building(Coords<uint32_t> tile_coords){
    for(size_t i=0;i<unfinished_buildings.size();i++){
        if(unfinished_buildings[i]==tile_coords){
            //Exit early, because this tile is already in the list
            return;
        }
    }

    unfinished_buildings.push_back(tile_coords);
    unfinished_building_flags.push_back(false);
}

void Civilization::remove_unfinished_building(Coords<uint32_t> tile_coords){
    for(size_t i=0;i<unfinished_buildings.size();i++){
        if(unfinished_buildings[i]==tile_coords){
            unfinished_buildings.erase(unfinished_buildings.begin()+i);
            unfinished_building_flags.erase(unfinished_building_flags.begin()+i);

            break;
        }
    }
}

bool Civilization::get_unfinished_building_flag(Coords<uint32_t> tile_coords) const{
    for(size_t i=0;i<unfinished_buildings.size();i++){
        if(unfinished_buildings[i]==tile_coords){
            return unfinished_building_flags[i];
        }
    }
}

void Civilization::set_unfinished_building_flag(Coords<uint32_t> tile_coords,bool new_flag){
    for(size_t i=0;i<unfinished_buildings.size();i++){
        if(unfinished_buildings[i]==tile_coords){
            unfinished_building_flags[i]=new_flag;

            break;
        }
    }
}

uint32_t Civilization::get_item_count() const{
    return inventory.get_item_count();
}

uint32_t Civilization::get_item_count(Inventory::Item_Type item_type) const{
    return inventory.get_item_count(item_type);
}

bool Civilization::has_food() const{
    return get_item_count(Inventory::Item_Type::WHEAT)>0;
}

uint32_t Civilization::add_item(Inventory::Item_Type item_type,uint32_t amount){
    return inventory.add_item(item_type,amount);
}

void Civilization::remove_item(Inventory::Item_Type item_type,uint32_t amount){
    inventory.remove_item(item_type,amount);
}

uint32_t Civilization::get_population() const{
    uint32_t population=0;

    for(size_t i=0;i<cities.size();i++){
        const City& city=Game::get_city(cities[i]);

        population+=city.get_population();
    }

    return population;
}

string Civilization::get_color() const{
    return Leader::get_color(get_parent_leader());
}

bool Civilization::is_friends_with(uint32_t civilization_index) const{
    const Leader& leader=Game::get_leader(get_parent_leader());

    const Civilization& civilization=Game::get_civilization(civilization_index);

    return leader.is_friends_with(civilization.get_parent_leader());
}

bool Civilization::is_enemies_with(uint32_t civilization_index) const{
    const Leader& leader=Game::get_leader(get_parent_leader());

    const Civilization& civilization=Game::get_civilization(civilization_index);

    return leader.is_enemies_with(civilization.get_parent_leader());
}

bool Civilization::is_neutral_towards(uint32_t civilization_index) const{
    const Leader& leader=Game::get_leader(get_parent_leader());

    const Civilization& civilization=Game::get_civilization(civilization_index);

    return leader.is_neutral_towards(civilization.get_parent_leader());
}

void Civilization::write_info_string(string& text) const{
    text+="Cities: "+Strings::num_to_string(cities.size())+"\n";

    text+="\n";

    text+="Population: "+Strings::num_to_string(get_population())+"\n";

    text+="\n";

    text+="Inventory:\n";

    vector<Inventory::Item_Type> item_types=Inventory::get_item_types();

    for(size_t i=0;i<item_types.size();i++){
        text+=Inventory::get_item_type_string(item_types[i])+": "+Strings::num_to_string(get_item_count(item_types[i]))+"\n";
    }
}
