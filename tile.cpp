/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "tile.h"
#include "game_constants.h"

#include <collision.h>
#include <render.h>
#include <game_manager.h>

///QQQ includes
#include "game.h"
#include <object_manager.h>
#include <font.h>
#include <engine_strings.h>
///

using namespace std;

Tile::Tile(){
    parent=0;

    type=Type::WHEAT;

    health=get_health_max();
}

Tile::Tile(Type new_type){
    parent=0;

    type=new_type;

    if(type==Type::BUILDING_UNFINISHED){
        health=1;
    }
    else{
        health=get_health_max();
    }
}

Tile::Tile(uint32_t new_parent,Type new_type){
    parent=new_parent;

    type=new_type;

    if(type==Type::BUILDING_UNFINISHED){
        health=1;
    }
    else{
        health=get_health_max();
    }
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

void Tile::set_type(Type new_type){
    type=new_type;

    health=get_health_max();
}

bool Tile::is_gatherable() const{
    return type==Type::WHEAT || type==Type::TREE;
}

bool Tile::is_food() const{
    return type==Type::WHEAT;
}

bool Tile::is_alive() const{
    return health>0;
}

bool Tile::needs_repair() const{
    return health<get_health_max();
}

void Tile::repair(){
    health+=Game_Constants::BUILDING_REPAIR_AMOUNT;

    if(health>get_health_max()){
        health=get_health_max();
    }
}

void Tile::capture_heal(){
    int16_t denominator=100/Game_Constants::BUILDING_CAPTURED_HEALTH_PERCENTAGE;

    if(denominator==0){
        denominator=1;
    }

    health=get_health_max()/denominator;

    if(health==0){
        health=1;
    }
}

void Tile::damage(int16_t attack){
    int16_t damage_done=attack-get_defense();

    if(damage_done<0){
        damage_done=0;
    }

    health-=damage_done;
}

void Tile::die(){
    health=0;
}

int16_t Tile::get_health_max() const{
    return Game_Constants::BUILDING_HEALTH_MAX;
}

int16_t Tile::get_health() const{
    return health;
}

int16_t Tile::get_defense() const{
    return Game_Constants::BUILDING_DEFENSE;
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
    if(is_alive()){
        //pixels
        double x=get_x(tile_x);
        double y=get_y(tile_y);

        Collision_Rect<double> box_render(x,y,(double)get_size(),(double)get_size());

        if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
            ///QQQ should render the tile graphic

            string color="white";
            if(type==Type::BUILDING_UNFINISHED){
                const Civilization& civilization=Game::get_civilization(get_parent());

                color=civilization.get_color();
            }
            else if(type==Type::BUILDING_CITY){
                const City& city=Game::get_city(get_parent());

                const Civilization& civilization=Game::get_civilization(city.get_parent_civilization());

                color=civilization.get_color();
            }

            Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (double)get_size()*Game_Manager::camera_zoom,(double)get_size()*Game_Manager::camera_zoom,1.0,color);

            ///QQQ dev data
            ///Bitmap_Font* font=Object_Manager::get_font("small");
            ///font->show(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,Strings::num_to_string(tile_x)+","+Strings::num_to_string(tile_y),"red");
            ///
        }
    }
}
