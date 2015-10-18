/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "minimap.h"
#include "game_constants.h"
#include "game.h"

#include <pixels.h>
#include <log.h>
#include <engine.h>
#include <object_manager.h>
#include <render.h>
#include <game_window.h>
#include <game_manager.h>

using namespace std;

bool Minimap::is_generated(){
    return surface!=0;
}

void Minimap::clear_surface(){
    if(surface!=0){
        SDL_FreeSurface(surface);
        surface=0;
    }
}

void Minimap::clear_image_data(){
    if(image_data.texture!=0){
        image_data.unload_image();
    }
}

Minimap::Minimap(){
    surface=0;

    width=0;
    height=0;

    chunk_width=0;
    chunk_height=0;
}

void Minimap::generate_surface(uint32_t new_width,uint32_t new_height){
    clear_surface();

    //These are the desired/maximum dimensions for the minimap
    //It may be scaled down some below due to rounding
    width=new_width;
    height=new_height;

    chunk_width=((double)width/double(Game::option_world_width*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE))*double(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
    chunk_height=((double)height/double(Game::option_world_height*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE))*double(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);

    ///QQQ What if one or both chunk dimensions are 0?

    width=chunk_width*Game::option_world_width;
    height=chunk_height*Game::option_world_height;

    uint32_t rmask=0;
    uint32_t gmask=0;
    uint32_t bmask=0;
    uint32_t amask=0;
    Engine::get_rgba_masks(&rmask,&gmask,&bmask,&amask);

    surface=SDL_CreateRGBSurface(0,(int)width,(int)height,32,rmask,gmask,bmask,amask);

    if(surface!=0){
        if(SDL_MUSTLOCK(surface)){
            if(SDL_LockSurface(surface)!=0){
                string msg="Error locking surface to generate minimap: ";
                msg+=SDL_GetError();
                Log::add_error(msg);
            }
        }

        for(uint32_t x=0;x<Game::option_world_width;x++){
            for(uint32_t y=0;y<Game::option_world_height;y++){
                const Chunk& chunk=Game::get_chunk(Coords<uint32_t>(x,y));

                Color* color=chunk.get_ground_color();

                for(uint32_t chunk_x=x*chunk_width;chunk_x<x*chunk_width+chunk_width;chunk_x++){
                    for(uint32_t chunk_y=y*chunk_height;chunk_y<y*chunk_height+chunk_height;chunk_y++){
                        Pixels::surface_put_pixel(surface,chunk_x,chunk_y,*color);
                    }
                }
            }
        }

        if(SDL_MUSTLOCK(surface)){
            SDL_UnlockSurface(surface);
        }
    }
    else{
        string msg="Error generating minimap: ";
        msg+=SDL_GetError();
        Log::add_error(msg);
    }
}

void Minimap::update(){
    if(is_generated()){
        clear_image_data();

        uint32_t rmask=0;
        uint32_t gmask=0;
        uint32_t bmask=0;
        uint32_t amask=0;
        Engine::get_rgba_masks(&rmask,&gmask,&bmask,&amask);

        SDL_Surface* surface_final=SDL_CreateRGBSurface(0,(int)width,(int)height,32,rmask,gmask,bmask,amask);

        if(surface_final!=0){
            double map_scale_x=(double)width/double(Game::option_world_width*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
            double map_scale_y=(double)height/double(Game::option_world_height*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);

            //pixels
            uint32_t city_width=(double(Game_Constants::BUILDING_SIZE*Game_Constants::TILE_SIZE)/double(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE))*(double)chunk_width;
            uint32_t city_height=(double(Game_Constants::BUILDING_SIZE*Game_Constants::TILE_SIZE)/double(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE))*(double)chunk_height;

            if(city_width==0){
                city_width=1;
            }
            if(city_height==0){
                city_height=1;
            }

            city_width*=8;
            city_height*=8;

            if(SDL_BlitSurface(surface,0,surface_final,0)!=0){
                string msg="Error blitting to surface for minimap update: ";
                msg+=SDL_GetError();
                Log::add_error(msg);
            }

            if(SDL_MUSTLOCK(surface_final)){
                if(SDL_LockSurface(surface_final)!=0){
                    string msg="Error locking surface to update minimap: ";
                    msg+=SDL_GetError();
                    Log::add_error(msg);
                }
            }

            const map<Coords<uint32_t>,Tile,Game::tile_compare>& tiles=Game::get_tiles();

            for(const auto& it : tiles){
                if(it.second.is_alive()){
                    bool render_tile=false;

                    if(it.second.get_type()==Tile::Type::BUILDING_UNFINISHED){
                        int32_t leader_index=Game::get_our_leader();
                        //If the tile is of type BUILDING_UNFINISHED, its parent is a Civilization
                        if(leader_index>=0 && it.second.get_parent()==(uint32_t)leader_index){
                            render_tile=true;
                        }
                    }
                    else if(!it.second.is_building()){
                        render_tile=true;
                    }

                    if(render_tile){
                        Color* color=Tile::get_type_color(it.second.get_type());

                        uint32_t x=(double)it.second.get_x(it.first.x)*map_scale_x;
                        uint32_t y=(double)it.second.get_y(it.first.y)*map_scale_y;

                        //pixels
                        uint32_t tile_width=((double)Tile::get_tile_type_size(it.second.get_type())/double(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE))*(double)chunk_width;
                        uint32_t tile_height=((double)Tile::get_tile_type_size(it.second.get_type())/double(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE))*(double)chunk_height;

                        if(tile_width==0){
                            tile_width=1;
                        }
                        if(tile_height==0){
                            tile_height=1;
                        }

                        if(it.second.get_type()==Tile::Type::BUILDING_UNFINISHED){
                            //If the tile is of type BUILDING_UNFINISHED, its parent is a Civilization
                            const Civilization& civilization=Game::get_civilization(it.second.get_parent());

                            color=Object_Manager::get_color(civilization.get_color());

                            tile_width*=4;
                            tile_height*=4;
                        }

                        for(uint32_t tile_x=x;tile_x<x+tile_width;tile_x++){
                            for(uint32_t tile_y=y;tile_y<y+tile_height;tile_y++){
                                if(tile_x<width && tile_y<height){
                                    Pixels::surface_put_pixel(surface_final,tile_x,tile_y,*color);
                                }
                            }
                        }
                    }
                }
            }

            for(uint32_t i=0;i<Game::get_city_count();i++){
                const City& city=Game::get_city(i);

                if(city.get_exists()){
                    const Civilization& civilization=Game::get_civilization(city.get_parent_civilization());

                    string color=civilization.get_color();

                    uint32_t x=(double)city.get_x()*map_scale_x;
                    uint32_t y=(double)city.get_y()*map_scale_y;

                    for(uint32_t city_x=x;city_x<x+city_width;city_x++){
                        for(uint32_t city_y=y;city_y<y+city_height;city_y++){
                            if(city_x<width && city_y<height){
                                Pixels::surface_put_pixel(surface_final,city_x,city_y,*Object_Manager::get_color(color));
                            }
                        }
                    }
                }
            }

            if(SDL_MUSTLOCK(surface_final)){
                SDL_UnlockSurface(surface_final);
            }

            image_data.load_image(surface_final);

            SDL_FreeSurface(surface_final);
        }
        else{
            string msg="Error updating minimap: ";
            msg+=SDL_GetError();
            Log::add_error(msg);
        }
    }
}

void Minimap::clear_map(){
    clear_surface();

    clear_image_data();
}

Collision_Rect<double> Minimap::get_box(){
    return Collision_Rect<double>((double)Game_Window::width()-(double)width-Game_Constants::RENDER_MINIMAP_BORDER,
                                  (double)Game_Window::height()-(double)height-Game_Constants::RENDER_MINIMAP_BORDER,
                                  (double)width,(double)height);
}

void Minimap::render(){
    Collision_Rect<double> box=get_box();

    double map_scale_x=(double)width/double(Game::option_world_width*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
    double map_scale_y=(double)height/double(Game::option_world_height*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);

    //Render the border
    Render::render_rectangle(box.x-Game_Constants::RENDER_MINIMAP_BORDER,box.y-Game_Constants::RENDER_MINIMAP_BORDER,
                             (double)width+Game_Constants::RENDER_MINIMAP_BORDER*2.0,(double)height+Game_Constants::RENDER_MINIMAP_BORDER*2.0,
                             1.0,"minimap_border");

    //Render the texture
    Render::render_texture(box.x,box.y,&image_data);

    //Render the camera rectangle
    Render::render_rectangle_empty(box.x+(Game_Manager::camera.x*map_scale_x)/Game_Manager::camera_zoom,
                                   box.y+(Game_Manager::camera.y*map_scale_y)/Game_Manager::camera_zoom,
                                   (Game_Manager::camera.w*map_scale_x)/Game_Manager::camera_zoom,(Game_Manager::camera.h*map_scale_y)/Game_Manager::camera_zoom,
                                   1.0,"minimap_camera_rectangle",Game_Constants::RENDER_MINIMAP_CAMERA_RECTANGLE);
}
