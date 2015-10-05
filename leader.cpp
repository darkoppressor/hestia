/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "leader.h"

using namespace std;

Leader::Leader(){
    parent_player=-1;

    civilization=0;
}

Leader::Leader(int32_t new_parent_player){
    parent_player=new_parent_player;

    civilization=0;
}

int32_t Leader::get_parent_player(){
    return parent_player;
}

void Leader::set_parent_player(int32_t new_parent_player){
    parent_player=new_parent_player;
}

uint32_t Leader::get_civilization(){
    return civilization;
}

void Leader::set_civilization(uint32_t new_civilization){
    civilization=new_civilization;
}

Color Leader::get_color(){
    return color;
}

void Leader::set_color(const Color& new_color){
    color=new_color;
}
