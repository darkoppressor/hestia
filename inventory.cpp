/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "inventory.h"

#include <log.h>
#include <engine_strings.h>
#include <engine.h>

using namespace std;

uint32_t& Inventory::get_item_reference(Item_Type type){
    if(type==Item_Type::WHEAT){
        return wheat;
    }
    else if(type==Item_Type::TREE){
        return tree;
    }
    else{
        Log::add_error("Error: Inventory::get_item_reference received unknown item type '"+Strings::num_to_string((uint8_t)type)+"'");

        Engine::quit();
    }
}

Inventory::Inventory(){
    wheat=0;
    tree=0;
}

vector<Inventory::Item_Type> Inventory::get_item_types(){
    vector<Item_Type> item_types;

    item_types.push_back(Item_Type::WHEAT);
    item_types.push_back(Item_Type::TREE);

    return item_types;
}

uint32_t Inventory::get_item_count() const{
    return wheat+tree;
}

uint32_t Inventory::get_item_count(Item_Type type) const{
    if(type==Item_Type::WHEAT){
        return wheat;
    }
    else if(type==Item_Type::TREE){
        return tree;
    }
    else{
        return 0;
    }
}

uint32_t Inventory::add_item(Item_Type type,uint32_t amount){
    uint32_t& item=get_item_reference(type);

    uint32_t available_space=UINT32_MAX-item;

    if(available_space>=amount){
        item+=amount;
    }
    else{
        item=UINT32_MAX;

        return amount-available_space;
    }

    return 0;
}

void Inventory::remove_item(Item_Type type,uint32_t amount){
    uint32_t& item=get_item_reference(type);

    if(amount<=item){
        item-=amount;
    }
    else{
        item=0;
    }
}
