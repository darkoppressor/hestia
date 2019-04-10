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

bool Game_Order::is_valid() const{
    const Leader& order_leader=Game::get_leader(leader);

    if(order_leader.is_defeated()){
        return false;
    }

    if(type==Type::BUILD_CITY){
        const Region& region = Game::get_region(Game::get_chunk(Coords<uint32_t>(Tile::get_chunk_x(coords.x),Tile::get_chunk_y(coords.y))).get_parent_region());

        if (!region.has_parent_civilization() && region.get_tile_count(Tile::Type::BUILDING_UNFINISHED) == 0) {
            return true;
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
    else if(type==Type::CANCEL_UNFINISHED_BUILDING){
        if(Game::tile_exists(coords)){
            const Tile& tile=Game::get_tile(coords);

            if(tile.is_alive() && tile.get_type()==Tile::Type::BUILDING_UNFINISHED){
                //If the tile is of type BUILDING_UNFINISHED, its parent is a Civilization
                const Civilization& civilization=Game::get_civilization(tile.get_parent());

                uint32_t leader_index=civilization.get_parent_leader();

                //If the tile is owned by the order's leader
                if(leader==leader_index){
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

            Coords<uint32_t> order_chunk_coords(Tile::get_chunk_x(coords.x),Tile::get_chunk_y(coords.y));
            Coords<uint32_t> building_tile_coords = Chunk::get_central_building_coords(order_chunk_coords);

            Game::new_tiles.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(building_tile_coords),std::forward_as_tuple(civilization_index,get_tile_type()));
        }
        else if(type==Type::REPOPULATE_CITY){
            Game::repopulate_city(coords.x);
        }
        else if(type==Type::ABANDON_CITY){
            Game::abandon_city(coords.x);
        }
        else if(type==Type::CANCEL_UNFINISHED_BUILDING){
            Game::kill_tile(coords);
        }
    }
}

void Game_Order::render() const{
    if(!Game::is_mouse_over_minimap()){
        bool valid = is_valid();

        string color="order_valid";
        if(!valid){
            color="order_invalid";
        }

        if(type==Type::BUILD_CITY){
            Coords<uint32_t> order_chunk_coords(Tile::get_chunk_x(coords.x),Tile::get_chunk_y(coords.y));
            const Region& region = Game::get_region(Game::get_chunk(order_chunk_coords).get_parent_region());

            region.render_chunk_border_overlays(color);

            Coords<uint32_t> building_tile_coords = Chunk::get_central_building_coords(order_chunk_coords);

            //pixels
            double x=Tile::get_x(building_tile_coords.x);
            double y=Tile::get_y(building_tile_coords.y);

            double tile_size=(double)Tile::get_tile_type_size(get_tile_type());

            Render::render_rectangle_empty(x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                           y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                           tile_size*Game_Manager::camera_zoom,
                                           tile_size*Game_Manager::camera_zoom,
                                           1.0,color,Game_Constants::RENDER_BUILDING_COLOR_BORDER*Game_Manager::camera_zoom);

            Render::render_texture(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                   Image_Manager::get_image("tile_"+Tile::get_type_string(get_tile_type())),0.75,Game_Manager::camera_zoom,Game_Manager::camera_zoom,0.0,color);

            if(!valid){
                Bitmap_Font* font=Object_Manager::get_font("standard");

                string message="Region is already occupied";

                double message_x=x+tile_size/2.0-((message.length()*font->spacing_x)/2.0)/Game_Manager::camera_zoom;

                font->show(message_x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                           (y+tile_size)*Game_Manager::camera_zoom-Game_Manager::camera.y,
                           message,"ui_white");
            }
        }
    }
}
