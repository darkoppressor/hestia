/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "tile.h"
#include "game_constants.h"

#include <collision.h>
#include <render.h>
#include <game_manager.h>

///QQQ
#include <object_manager.h>
#include <font.h>
#include <engine_strings.h>
///

using namespace std;

Tile::Tile(){
    parent=0;

    type=Type::WHEAT;

    alive=true;
}

Tile::Tile(Type new_type){
    parent=0;

    type=new_type;

    alive=true;
}

Tile::Tile(uint32_t new_parent,Type new_type){
    parent=new_parent;

    type=new_type;

    alive=true;
}

uint32_t Tile::get_parent() const{
    return parent;
}

void Tile::set_parent(uint32_t new_parent){
    parent=new_parent;
}

Tile::Type Tile::get_type() const{
    return type;
}

bool Tile::is_gatherable() const{
    return type==Type::WHEAT || type==Type::TREE;
}

bool Tile::is_alive() const{
    return alive;
}

void Tile::kill(){
    alive=false;
}

bool Tile::tile_type_is_building(Type type_to_check){
    if(type_to_check==Type::BUILDING_CITY || type_to_check==Type::BUILDING_UNFINISHED){
        return true;
    }
    else{
        return false;
    }
}

bool Tile::is_building() const{
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

int32_t Tile::get_size() const{
    return get_tile_type_size(type);
}

int32_t Tile::get_x(uint32_t tile_x){
    return int32_t(tile_x*Game_Constants::TILE_SIZE);
}

int32_t Tile::get_y(uint32_t tile_y){
    return int32_t(tile_y*Game_Constants::TILE_SIZE);
}

int32_t Tile::get_center_x(uint32_t tile_x,int32_t tile_size){
    return int32_t(tile_x*Game_Constants::TILE_SIZE)+tile_size/2;
}

int32_t Tile::get_center_y(uint32_t tile_y,int32_t tile_size){
    return int32_t(tile_y*Game_Constants::TILE_SIZE)+tile_size/2;
}

int32_t Tile::get_center_x(uint32_t tile_x) const{
    return get_center_x(tile_x,get_size());
}

int32_t Tile::get_center_y(uint32_t tile_y) const{
    return get_center_y(tile_y,get_size());
}

uint32_t Tile::get_chunk_x(uint32_t tile_x){
    return tile_x/Game_Constants::CHUNK_SIZE;
}

uint32_t Tile::get_chunk_y(uint32_t tile_y){
    return tile_y/Game_Constants::CHUNK_SIZE;
}

void Tile::render(uint32_t tile_x,uint32_t tile_y) const{
    if(alive){
        double x=get_x(tile_x);
        double y=get_y(tile_y);

        Collision_Rect<double> box_render(x,y,(double)get_size(),(double)get_size());

        if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
            Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (double)get_size()*Game_Manager::camera_zoom,(double)get_size()*Game_Manager::camera_zoom,1.0,"white");

            ///QQQ
            ///Bitmap_Font* font=Object_Manager::get_font("small");
            ///font->show(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,Strings::num_to_string(tile_x)+","+Strings::num_to_string(tile_y),"red");
            ///
        }
    }
}
