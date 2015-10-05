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

using namespace std;

Chunk::Chunk(){
    parent_region=0;
}

int32_t Chunk::get_x(uint32_t chunk_x){
    return (int32_t)(chunk_x*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

int32_t Chunk::get_y(uint32_t chunk_y){
    return (int32_t)(chunk_y*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

Region* Chunk::get_parent_region(){
    if(parent_region>=Game::regions.size()){
        Log::add_error("Error in Chunk::get_parent_region()");

        return 0;
    }
    else{
        return &Game::regions[parent_region];
    }
}

string Chunk::get_ground_string(){
    Region* region=get_parent_region();

    if(region!=0){
        return region->get_ground_string(region->get_ground());
    }
    else{
        return "stone";
    }
}

void Chunk::set_parent_region(uint32_t new_parent_region){
    parent_region=new_parent_region;
}

void Chunk::render_ground(uint32_t chunk_x,uint32_t chunk_y){
    double x=get_x(chunk_x);
    double y=get_y(chunk_y);

    Collision_Rect<double> box_render(x,y,Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE,Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_texture(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,Image_Manager::get_image("ground_"+get_ground_string()),1.0,Game_Manager::camera_zoom,Game_Manager::camera_zoom);
    }
}
