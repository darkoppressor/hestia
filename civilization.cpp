/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "civilization.h"
#include "game.h"
#include "game_constants.h"

#include <engine_strings.h>
#include <engine.h>

using namespace std;

Civilization::Civilization(){
    parent_leader=0;

    need_wheat=0;
    need_tree=0;

    surplus_wheat=0;
    surplus_tree=0;
}

Civilization::Civilization(uint32_t new_parent){
    parent_leader=new_parent;

    need_wheat=0;
    need_tree=0;

    surplus_wheat=0;
    surplus_tree=0;
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

    return false;
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

        if(city.get_exists()){
            population+=city.get_population();
        }
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

uint32_t Civilization::get_item_need(Inventory::Item_Type item_type) const{
    if(item_type==Inventory::Item_Type::WHEAT){
        return need_wheat;
    }
    else if(item_type==Inventory::Item_Type::TREE){
        return need_tree;
    }
    else{
        return 0;
    }
}

uint32_t Civilization::get_item_desire(Inventory::Item_Type item_type) const{
    if(item_type==Inventory::Item_Type::WHEAT){
        return surplus_wheat;
    }
    else if(item_type==Inventory::Item_Type::TREE){
        return surplus_tree;
    }
    else{
        return 0;
    }
}

bool Civilization::is_item_needed(Inventory::Item_Type item_type) const{
    return get_item_need(item_type)>0;
}

bool Civilization::is_item_desired(Inventory::Item_Type item_type) const{
    return get_item_desire(item_type)>0;
}

bool Civilization::allowed_to_update_needs(uint32_t frame,uint32_t index) const{
    if((frame+(index%Engine::UPDATE_RATE))%Game_Constants::CIVILIZATION_NEEDS_UPDATE_PERIOD==0){
        return true;
    }
    else{
        return false;
    }
}

void Civilization::update_needs(uint32_t frame,uint32_t index){
    if(allowed_to_update_needs(frame,index)){
        need_wheat=0;
        need_tree=0;

        surplus_wheat=Game_Constants::SURPLUS_BASE_WHEAT;
        surplus_tree=Game_Constants::SURPLUS_BASE_TREE;

        //Determine needed item counts

        uint32_t population=get_population();

        need_wheat+=population;

        need_tree+=(uint32_t)unfinished_buildings.size()*Game_Constants::COST_BUILD;

        uint32_t repairs_needed=0;

        for(size_t i=0;i<cities.size();i++){
            const City& city=Game::get_city(cities[i]);

            if(city.get_exists()){
                repairs_needed+=city.repair_count_needed();
            }
        }

        need_tree+=repairs_needed*Game_Constants::COST_REPAIR;

        //Determine desired surpluses

        uint32_t denominator=100/Game_Constants::SURPLUS_WHEAT;
        surplus_wheat+=need_wheat/denominator;

        denominator=100/Game_Constants::SURPLUS_TREE;
        surplus_tree+=need_tree/denominator;

        //Adjust needs and desired surpluses based on items currently held

        uint32_t item_count_wheat=get_item_count(Inventory::Item_Type::WHEAT);
        uint32_t item_count_tree=get_item_count(Inventory::Item_Type::TREE);

        if(need_wheat<=item_count_wheat){
            uint32_t adjusted_item_count=item_count_wheat-need_wheat;

            if(surplus_wheat<=adjusted_item_count){
                surplus_wheat=0;
            }
            else{
                surplus_wheat-=adjusted_item_count;
            }

            need_wheat=0;
        }
        else{
            need_wheat-=item_count_wheat;
        }

        if(need_tree<=item_count_tree){
            uint32_t adjusted_item_count=item_count_tree-need_tree;

            if(surplus_tree<=adjusted_item_count){
                surplus_tree=0;
            }
            else{
                surplus_tree-=adjusted_item_count;
            }

            need_tree=0;
        }
        else{
            need_tree-=item_count_tree;
        }
    }
}

void Civilization::write_info_string(string& text) const{
    text+="Cities: "+Strings::num_to_string(cities.size())+"\n";

    text+="\n";

    text+="Population: "+Strings::num_to_string(get_population())+"\n";

    text+="\n";

    text+="Inventory:\n";

    vector<Inventory::Item_Type> item_types=Inventory::get_item_types();

    for(size_t i=0;i<item_types.size();i++){
        text+=Inventory::get_item_type_string(item_types[i])+": "+Strings::num_to_string(get_item_count(item_types[i]));

        uint32_t item_need=get_item_need(item_types[i]);
        uint32_t item_desire=get_item_desire(item_types[i]);
        uint32_t item_total=item_need+item_desire;

        if(item_total>0){
            text+=" ("+Strings::num_to_string(item_total)+" desired)";
        }

        text+="\n";
    }
}
