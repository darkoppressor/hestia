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

vector<Leader::Diplomacy_State> Leader::get_diplomacy_states() const{
    return diplomacy_states;
}

void Leader::set_diplomacy_states(const vector<Diplomacy_State>& new_diplomacy_states){
    diplomacy_states=new_diplomacy_states;
}

void Leader::initialize_diplomacy_states(uint32_t index,uint32_t leader_count){
    for(uint32_t i=0;i<leader_count;i++){
        if(i!=index){
            diplomacy_states.push_back(Diplomacy_State::IS_ENEMIES_WITH);
        }
        else{
            diplomacy_states.push_back(Diplomacy_State::IS_FRIENDS_WITH);
        }
    }
}

bool Leader::is_friends_with(uint32_t leader_index) const{
    if(leader_index<diplomacy_states.size()){
        if(diplomacy_states[leader_index]==Diplomacy_State::IS_FRIENDS_WITH){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool Leader::is_enemies_with(uint32_t leader_index) const{
    if(leader_index<diplomacy_states.size()){
        if(diplomacy_states[leader_index]==Diplomacy_State::IS_ENEMIES_WITH){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool Leader::is_neutral_towards(uint32_t leader_index) const{
    if(leader_index<diplomacy_states.size()){
        if(diplomacy_states[leader_index]==Diplomacy_State::IS_NEUTRAL_TOWARDS){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
