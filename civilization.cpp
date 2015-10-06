/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "civilization.h"

using namespace std;

Civilization::Civilization(){
    parent_leader=0;
}

Civilization::Civilization(uint32_t new_parent){
    parent_leader=new_parent;
}

uint32_t Civilization::get_parent_leader(){
    return parent_leader;
}

void Civilization::set_parent_leader(uint32_t new_parent){
    parent_leader=new_parent;
}

const vector<uint32_t>& Civilization::get_cities(){
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
