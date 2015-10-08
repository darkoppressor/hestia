/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "chunk.h"
#include "game_constants.h"
#include "game.h"

#include <log.h>
#include <collision.h>
#include <render.h>
#include <game_manager.h>
#include <image_manager.h>
#include <engine_strings.h>
#include <engine.h>

using namespace std;

uint32_t& Tile_Counts::get_tile_reference(Tile::Type tile_type){
    if(tile_type==Tile::Type::WHEAT){
        return wheat;
    }
    else if(tile_type==Tile::Type::TREE){
        return tree;
    }
    else if(tile_type==Tile::Type::BUILDING_UNFINISHED){
        return building_unfinished;
    }
    else if(tile_type==Tile::Type::BUILDING_CITY){
        return building_city;
    }
    else{
        Log::add_error("Error: Tile_Counts::get_tile_reference received unknown tile type '"+Strings::num_to_string((uint8_t)tile_type)+"'");

        Engine::quit();
    }
}

Tile_Counts::Tile_Counts(){
    wheat=0;
    tree=0;
    building_unfinished=0;
    building_city=0;
}

uint32_t Tile_Counts::get(Tile::Type tile_type) const{
    if(tile_type==Tile::Type::WHEAT){
        return wheat;
    }
    else if(tile_type==Tile::Type::TREE){
        return tree;
    }
    else if(tile_type==Tile::Type::BUILDING_UNFINISHED){
        return building_unfinished;
    }
    else if(tile_type==Tile::Type::BUILDING_CITY){
        return building_city;
    }
    else{
        return 0;
    }
}

void Tile_Counts::increment(Tile::Type tile_type){
    uint32_t& tile=get_tile_reference(tile_type);

    if(tile<UINT32_MAX){
        tile++;
    }
}

void Tile_Counts::decrement(Tile::Type tile_type){
    uint32_t& tile=get_tile_reference(tile_type);

    if(tile>0){
        tile--;
    }
}

Chunk::Chunk(){
    parent_region=0;
}

uint32_t Chunk::get_parent_region() const{
    return parent_region;
}

void Chunk::set_parent_region(uint32_t new_parent){
    parent_region=new_parent;
}

uint32_t Chunk::get_tile_count(Tile::Type tile_type) const{
    return tile_counts.get(tile_type);
}

void Chunk::increment_tile_count(Tile::Type tile_type){
    tile_counts.increment(tile_type);
}

void Chunk::decrement_tile_count(Tile::Type tile_type){
    tile_counts.decrement(tile_type);
}

string Chunk::get_ground_string() const{
    const Region& region=Game::get_region(get_parent_region());

    return region.get_ground_string(region.get_ground());
}

int32_t Chunk::get_size() const{
    return Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE;
}

int32_t Chunk::get_x(uint32_t chunk_x){
    return int32_t(chunk_x*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

int32_t Chunk::get_y(uint32_t chunk_y){
    return int32_t(chunk_y*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

void Chunk::render_ground(uint32_t chunk_x,uint32_t chunk_y) const{
    double x=get_x(chunk_x);
    double y=get_y(chunk_y);

    Collision_Rect<double> box_render(x,y,(double)get_size(),(double)get_size());

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_texture(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                               Image_Manager::get_image("ground_"+get_ground_string()),1.0,Game_Manager::camera_zoom,Game_Manager::camera_zoom);
    }
}
