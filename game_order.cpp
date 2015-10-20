/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_order.h"
#include "game.h"
#include "game_constants.h"

#include <render.h>
#include <game_manager.h>
#include <image_manager.h>
#include <font.h>
#include <object_manager.h>

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

bool Game_Order::is_city_build_area_clear() const{
    Collision_Rect<uint32_t> city_spacing_area=Game::get_city_spacing_area(coords);

    bool city_within_city_space=false;

    for(uint32_t x=city_spacing_area.x;x<=city_spacing_area.x+city_spacing_area.w && !city_within_city_space;x++){
        for(uint32_t y=city_spacing_area.y;y<=city_spacing_area.y+city_spacing_area.h && !city_within_city_space;y++){
            Coords<uint32_t> tile_coords(x,y);

            if(Game::tile_exists(tile_coords)){
                const Tile& tile=Game::get_tile(tile_coords);

                if(tile.is_alive() && tile.get_type()==Tile::Type::BUILDING_CITY){
                    city_within_city_space=true;
                }
            }
        }
    }

    if(!city_within_city_space){
        return true;
    }
    else{
        return false;
    }
}

bool Game_Order::is_valid() const{
    if(type==Type::BUILD_CITY){
        if(Game::tile_coords_are_valid(get_tile_type(),coords)){
            return is_city_build_area_clear();
        }

        return false;
    }
    else if(type==Type::REPOPULATE_CITY){
        uint32_t city_index=coords.x;

        if(city_index<Game::get_city_count()){
            const City& city=Game::get_city(city_index);

            if(city.get_exists()){
                const Civilization& civilization=Game::get_civilization(city.get_parent_civilization());

                uint32_t leader_index=civilization.get_parent_leader();

                //If the city is owned by the order's leader
                if(leader==leader_index){
                    return true;
                }
            }
        }

        return false;
    }
    else if(type==Type::ABANDON_CITY){
        uint32_t city_index=coords.x;

        if(city_index<Game::get_city_count()){
            const City& city=Game::get_city(city_index);

            if(city.get_exists()){
                const Civilization& civilization=Game::get_civilization(city.get_parent_civilization());

                uint32_t leader_index=civilization.get_parent_leader();

                //If the city is owned by the order's leader and its parent civilization has at least one other city
                if(leader==leader_index && civilization.get_cities().size()>1){
                    return true;
                }
            }
        }

        return false;
    }
    else{
        return false;
    }
}

void Game_Order::execute(){
    if(is_valid()){
        if(type==Type::BUILD_CITY){
            const Leader& order_leader=Game::get_leader(leader);

            uint32_t civilization_index=order_leader.get_civilization();

            Game::new_tiles.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(coords),std::forward_as_tuple(civilization_index,get_tile_type()));
        }
        else if(type==Type::REPOPULATE_CITY){
            Game::repopulate_city(coords.x);
        }
        else if(type==Type::ABANDON_CITY){
            Game::abandon_city(coords.x);
        }
    }
}

void Game_Order::render() const{
    if(!Game::is_mouse_over_minimap()){
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

            if(!is_valid() && !is_city_build_area_clear()){
                Bitmap_Font* font=Object_Manager::get_font("standard");

                string message="Too close to building";

                double message_x=x+tile_size/2.0-((message.length()*font->spacing_x)/2.0)/Game_Manager::camera_zoom;

                font->show(message_x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                           (y+tile_size)*Game_Manager::camera_zoom-Game_Manager::camera.y,
                           message,"ui_white");
            }
        }
    }
}
