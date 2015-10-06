/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "tile.h"
#include "game_constants.h"

#include <collision.h>
#include <render.h>
#include <game_manager.h>

using namespace std;

Tile::Tile(){
    parent=0;

    type=Type::WHEAT;
}

Tile::Tile(Type new_type){
    parent=0;

    type=new_type;
}

Tile::Tile(uint32_t new_parent,Type new_type){
    parent=new_parent;

    type=new_type;
}

uint32_t Tile::get_parent(){
    return parent;
}

void Tile::set_parent(uint32_t new_parent){
    parent=new_parent;
}

bool Tile::tile_type_is_building(Type type_to_check){
    if(type_to_check==Type::BUILDING_CITY || type_to_check==Type::BUILDING_UNFINISHED){
        return true;
    }
    else{
        return false;
    }
}

bool Tile::is_building(){
    if(tile_type_is_building(type)){
        return true;
    }
    else{
        return false;
    }
}

int32_t Tile::get_tile_type_size(Type type_to_check){
    if(tile_type_is_building(type_to_check)){
        return int32_t(Game_Constants::BUILDING_SIZE*Game_Constants::TILE_SIZE);
    }
    else{
        return (int32_t)Game_Constants::TILE_SIZE;
    }
}

int32_t Tile::get_size(){
    return get_tile_type_size(type);
}

int32_t Tile::get_x(uint32_t tile_x){
    return int32_t(tile_x*Game_Constants::TILE_SIZE);
}

int32_t Tile::get_y(uint32_t tile_y){
    return int32_t(tile_y*Game_Constants::TILE_SIZE);
}

int32_t Tile::get_center_x(uint32_t tile_x){
    return int32_t(tile_x*Game_Constants::TILE_SIZE)+get_size()/2;
}

int32_t Tile::get_center_y(uint32_t tile_y){
    return int32_t(tile_y*Game_Constants::TILE_SIZE)+get_size()/2;
}

void Tile::render(uint32_t tile_x,uint32_t tile_y){
    double x=get_x(tile_x);
    double y=get_y(tile_y);

    Collision_Rect<double> box_render(x,y,(double)get_size(),(double)get_size());

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                 (double)get_size()*Game_Manager::camera_zoom,(double)get_size()*Game_Manager::camera_zoom,1.0,"white");
    }
}
