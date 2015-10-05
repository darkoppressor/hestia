/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "civilization.h"

using namespace std;

Civilization::Civilization(){
    parent_leader=0;
}

Civilization::Civilization(uint32_t new_parent_leader){
    parent_leader=new_parent_leader;
}

uint32_t Civilization::get_parent_leader(){
    return parent_leader;
}

void Civilization::set_parent_leader(uint32_t new_parent_leader){
    parent_leader=new_parent_leader;
}
