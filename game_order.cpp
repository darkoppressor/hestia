/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_order.h"
#include "game.h"
#include "game_constants.h"

#include <render.h>
#include <game_manager.h>
#include <image_manager.h>

using namespace std;

Game_Order::Game_Order(Type new_type,const Coords<uint32_t>& new_coords,uint32_t new_leader){
    type=new_type;

    coords=new_coords;

    leader=new_leader;
}

Tile::Type Game_Order::get_tile_type() const{
    if(type==Type::BUILD_CITY){
        return Tile::Type::BUILDING_UNFINISHED;
    }
    else{
        return Tile::Type::WHEAT;
    }
}

Coords<int32_t> Game_Order::get_pixel_coords() const{
    return Coords<int32_t>(Tile::get_center_x(coords.x,Tile::get_tile_type_size(get_tile_type())),
                           Tile::get_center_y(coords.y,Tile::get_tile_type_size(get_tile_type())));
}

bool Game_Order::is_valid() const{
    if(type==Type::BUILD_CITY){
        if(Game::tile_coords_are_valid(get_tile_type(),coords)){
            uint32_t city_spacing=Game_Constants::CITY_SPACING;

            uint32_t tile_start_x=coords.x;
            uint32_t tile_start_y=coords.y;

            if(tile_start_x>city_spacing){
                tile_start_x-=city_spacing;
            }
            else{
                tile_start_x=0;
            }

            if(tile_start_y>city_spacing){
                tile_start_y-=city_spacing;
            }
            else{
                tile_start_y=0;
            }

            uint32_t tile_end_x=tile_start_x+city_spacing*2;
            uint32_t tile_end_y=tile_start_y+city_spacing*2;

            if(tile_end_x>=Game::get_world_width_tiles()){
                tile_end_x=Game::get_world_width_tiles()-1;
            }
            if(tile_end_y>=Game::get_world_height_tiles()){
                tile_end_y=Game::get_world_height_tiles()-1;
            }

            bool building_within_city_space=false;

            for(uint32_t x=tile_start_x;x<=tile_end_x && !building_within_city_space;x++){
                for(uint32_t y=tile_start_y;y<=tile_end_y && !building_within_city_space;y++){
                    Coords<uint32_t> tile_coords(x,y);

                    if(Game::tile_exists(tile_coords)){
                        const Tile& tile=Game::get_tile(tile_coords);

                        if(tile.is_alive() && tile.is_building()){
                            building_within_city_space=true;
                        }
                    }
                }
            }

            if(!building_within_city_space){
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
    else{
        return false;
    }
}

void Game_Order::execute(){
    if(type==Type::BUILD_CITY){
        const Leader& order_leader=Game::get_leader(leader);

        uint32_t civilization_index=order_leader.get_civilization();

        Game::new_tiles.emplace(std::piecewise_construct,
                                std::forward_as_tuple(coords),std::forward_as_tuple(civilization_index,get_tile_type()));
    }
}

void Game_Order::render() const{
    //pixels
    double x=Tile::get_x(coords.x);
    double y=Tile::get_y(coords.y);

    string color="order_valid";
    if(!is_valid()){
        color="order_invalid";
    }

    if(type==Type::BUILD_CITY){
        double tile_size=(double)Tile::get_tile_type_size(get_tile_type());

        Render::render_rectangle_empty(x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                       y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                       tile_size*Game_Manager::camera_zoom,
                                       tile_size*Game_Manager::camera_zoom,
                                       1.0,color,Game_Constants::RENDER_BUILDING_COLOR_BORDER*Game_Manager::camera_zoom);

        Render::render_texture(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                               Image_Manager::get_image("tile_building_unfinished"),0.75,Game_Manager::camera_zoom,Game_Manager::camera_zoom,0.0,color);
    }
}