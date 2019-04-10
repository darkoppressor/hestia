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

    if(tile<numeric_limits<uint32_t>::max()){
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

void Chunk::add_checksum_data(vector<uint32_t>& data) const{
    data.push_back(parent_region);

    data.push_back(tile_counts.get(Tile::Type::WHEAT));
    data.push_back(tile_counts.get(Tile::Type::TREE));
    data.push_back(tile_counts.get(Tile::Type::BUILDING_UNFINISHED));
    data.push_back(tile_counts.get(Tile::Type::BUILDING_CITY));
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

bool Chunk::has_food() const{
    return get_tile_count(Tile::Type::WHEAT)>0;
}

string Chunk::get_ground_string() const{
    const Region& region=Game::get_region(get_parent_region());

    return region.get_ground_string();
}

Color* Chunk::get_ground_color() const{
    const Region& region=Game::get_region(get_parent_region());

    return region.get_ground_color();
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

Coords<uint32_t> Chunk::get_central_building_coords(const Coords<uint32_t>& chunk_coords){
    return Coords<uint32_t>(chunk_coords.x*Game_Constants::CHUNK_SIZE+Game_Constants::CHUNK_SIZE/2-Game_Constants::BUILDING_SIZE/2,chunk_coords.y*Game_Constants::CHUNK_SIZE+Game_Constants::CHUNK_SIZE/2-Game_Constants::BUILDING_SIZE/2);
}

vector<Coords<uint32_t>> Chunk::get_zone_chunk_coords(uint32_t chunk_x,uint32_t chunk_y,uint32_t zone_range){
    //chunks
    uint32_t zone_x=chunk_x;
    uint32_t zone_y=chunk_y;
    uint32_t zone_width=zone_range*2;
    uint32_t zone_height=zone_range*2;

    if(zone_x>=zone_range){
        zone_x-=zone_range;
    }
    else{
        zone_x=0;
    }

    if(zone_y>=zone_range){
        zone_y-=zone_range;
    }
    else{
        zone_y=0;
    }

    if(zone_x+zone_width>=Game::option_world_width){
        zone_width=Game::option_world_width-1-zone_x;
    }
    if(zone_y+zone_height>=Game::option_world_height){
        zone_height=Game::option_world_height-1-zone_y;
    }

    Collision_Rect<uint32_t> zone(zone_x,zone_y,zone_width,zone_height);

    //A list of chunk coordinates within the zone
    vector<Coords<uint32_t>> chunk_coords;

    if(zone_range>0){
        for(uint32_t x=zone.x;x<zone.x+zone.w;x++){
            for(uint32_t y=zone.y;y<zone.y+zone.h;y++){
                chunk_coords.push_back(Coords<uint32_t>(x,y));
            }
        }
    }
    else{
        chunk_coords.push_back(Coords<uint32_t>(zone.x,zone.y));
    }

    return chunk_coords;
}

void Chunk::render_ground(uint32_t chunk_x,uint32_t chunk_y) const{
    double x=get_x(chunk_x);
    double y=get_y(chunk_y);

    Collision_Rect<double> box_render(x,y,(double)get_size(),(double)get_size());

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_texture(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                               Image_Manager::get_image("ground_"+get_ground_string()),1.0,Game_Manager::camera_zoom,Game_Manager::camera_zoom);

        ///QQQ dev data
        /**Render::render_rectangle_empty(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                       get_size()*Game_Manager::camera_zoom,get_size()*Game_Manager::camera_zoom,0.85,"red",1.0);*/
        ///
    }
}

void Chunk::render_border_overlay(uint32_t chunk_x,uint32_t chunk_y,const string& color) const{
    double x=get_x(chunk_x);
    double y=get_y(chunk_y);

    Collision_Rect<double> box_render(x,y,(double)get_size(),(double)get_size());

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                       get_size()*Game_Manager::camera_zoom,get_size()*Game_Manager::camera_zoom,0.2,color);

        Render::render_rectangle_empty(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                       get_size()*Game_Manager::camera_zoom,get_size()*Game_Manager::camera_zoom,1.0,color,Game_Constants::RENDER_CHUNK_BORDER_THICKNESS*Game_Manager::camera_zoom);
    }
}
