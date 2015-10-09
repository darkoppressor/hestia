/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "leader.h"

#include <engine_strings.h>

using namespace std;

Leader::Leader(){
    player_controlled=false;
    parent_player=0;

    civilization=0;
}

Leader::Leader(uint32_t new_parent){
    player_controlled=true;
    parent_player=new_parent;

    civilization=0;
}

bool Leader::is_player_controlled() const{
    return player_controlled;
}

uint32_t Leader::get_parent_player() const{
    return parent_player;
}

void Leader::set_parent_player(uint32_t new_parent){
    player_controlled=true;
    parent_player=new_parent;
}

void Leader::set_parent_to_ai(){
    player_controlled=false;
    parent_player=0;
}

uint32_t Leader::get_civilization() const{
    return civilization;
}

void Leader::set_civilization(uint32_t new_civilization){
    civilization=new_civilization;
}

Color Leader::get_color() const{
    return color;
}

string Leader::get_color(uint32_t index){
    return "leader_"+Strings::num_to_string(index);
}

void Leader::set_color(const Color& new_color){
    color=new_color;
}
