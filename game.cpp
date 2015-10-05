/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_constants.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <network_lockstep.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <network_engine.h>

using namespace std;

bool Game::started=false;

uint32_t Game::option_rng_seed=0;
uint32_t Game::option_world_width=0;
uint32_t Game::option_world_height=0;
uint32_t Game::option_region_min=0;
uint32_t Game::option_region_max=0;
uint32_t Game::option_max_leaders=0;

RNG Game::rng;

vector<Region> Game::regions;
vector<vector<Chunk>> Game::chunks;
vector<Leader> Game::leaders;
vector<Civilization> Game::civilizations;
vector<City> Game::cities;
vector<Person> Game::people;
map<Coords<uint32_t>,Tile> Game::tiles;

uint32_t Game::get_world_width(){
    return option_world_width*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE;
}

uint32_t Game::get_world_height(){
    return option_world_height*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE;
}

void Game::clear_world(){
    started=false;

    option_rng_seed=0;
    option_world_width=5;
    option_world_height=5;
    option_region_min=4;
    option_region_max=8;

    option_max_leaders=2;

    regions.clear();
    chunks.clear();
    leaders.clear();
    civilizations.clear();
    cities.clear();
    people.clear();
    tiles.clear();
}

void Game::setup_leaders(){
    RNG rng_color;

    uint32_t player_count=Network_Engine::get_player_count();

    for(uint32_t i=0;i<option_max_leaders;i++){
        int32_t parent_player=-1;

        if(i<player_count){
            parent_player=i;
        }

        leaders.push_back(Leader(parent_player));
        leaders.back().set_color(Color(rng_color.random_range(0,255),rng_color.random_range(0,255),rng_color.random_range(0,255),255));
    }
}

void Game::generate_world(){
    clear_world();

    rng.seed(option_rng_seed);

    chunks.resize(option_world_width,vector<Chunk>(option_world_height));
    vector<vector<bool>> chunks_in_regions;
    chunks_in_regions.resize(option_world_width,vector<bool>(option_world_height));

    for(uint32_t x=0;x<option_world_width;x++){
        for(uint32_t y=0;y<option_world_height;y++){
            chunks_in_regions[x][y]=false;
        }
    }

    for(uint32_t x=0;x<option_world_width;x++){
        for(uint32_t y=0;y<option_world_height;y++){
            //If this chunk is not already in a region
            if(!chunks_in_regions[x][y]){
                //Start a new region
                regions.push_back(Region());

                //Add this chunk to the new region
                regions.back().add_chunk(Coords<uint32_t>(x,y));
                chunks[x][y].set_parent_region(regions.size()-1);
                chunks_in_regions[x][y]=true;

                //Keep a running count of the chunks in this new region
                uint32_t chunks_in_region=1;

                //The maximum size of this region
                uint32_t region_max=rng.random_range(option_region_min,option_region_max);

                //Keep track of the currently checked chunk
                uint32_t check_x=x;
                uint32_t check_y=y;

                //Add connected chunks to this region
                for(uint32_t iteration=0,distance_from_start=0;chunks_in_region<region_max && iteration<100;iteration++){
                    //Choose a random direction
                    string rand_direction=Game_Manager::get_random_direction_cardinal(rng);

                    //If the chunk in that direction is within the world bounds and not already in a region
                    if(rand_direction=="left" && check_x>0 && !chunks_in_regions[check_x-1][check_y]){
                        //Add the chunk to the region
                        regions.back().add_chunk(Coords<uint32_t>(check_x-1,check_y));
                        chunks[check_x-1][check_y].set_parent_region(regions.size()-1);
                        chunks_in_regions[check_x-1][check_y]=true;

                        check_x--;
                        distance_from_start++;
                        chunks_in_region++;
                    }
                    else if(rand_direction=="right" && check_x<option_world_width-1 && !chunks_in_regions[check_x+1][check_y]){
                        //Add the chunk to the region
                        regions.back().add_chunk(Coords<uint32_t>(check_x+1,check_y));
                        chunks[check_x+1][check_y].set_parent_region(regions.size()-1);
                        chunks_in_regions[check_x+1][check_y]=true;

                        check_x++;
                        distance_from_start++;
                        chunks_in_region++;
                    }
                    else if(rand_direction=="up" && check_y>0 && !chunks_in_regions[check_x][check_y-1]){
                        //Add the chunk to the region
                        regions.back().add_chunk(Coords<uint32_t>(check_x,check_y-1));
                        chunks[check_x][check_y-1].set_parent_region(regions.size()-1);
                        chunks_in_regions[check_x][check_y-1]=true;

                        check_y--;
                        distance_from_start++;
                        chunks_in_region++;
                    }
                    else if(rand_direction=="down" && check_y<option_world_height-1 && !chunks_in_regions[check_x][check_y+1]){
                        //Add the chunk to the region
                        regions.back().add_chunk(Coords<uint32_t>(check_x,check_y+1));
                        chunks[check_x][check_y+1].set_parent_region(regions.size()-1);
                        chunks_in_regions[check_x][check_y+1]=true;

                        check_y++;
                        distance_from_start++;
                        chunks_in_region++;
                    }

                    //If we have added a few chunks away from our starting chunk
                    if(distance_from_start>8){
                        //There is a chance we will reset to the starting chunk
                        if(rng.random_range(0,99)<50){
                            check_x=x;
                            check_y=y;
                            distance_from_start=0;
                        }
                    }
                }
            }
        }
    }

    //Set region biomes
    for(size_t i=0;i<regions.size();i++){
        uint32_t random=rng.random_range(0,8);

        if(random==0){
            regions[i].set_biome(Region::Biome::FREEZING_GRASSLAND);
        }
        else if(random==1){
            regions[i].set_biome(Region::Biome::TROPICAL_GRASSLAND);
        }
        else if(random==2){
            regions[i].set_biome(Region::Biome::TEMPERATE_GRASSLAND);
        }
        else if(random==3){
            regions[i].set_biome(Region::Biome::FREEZING_FOREST);
        }
        else if(random==4){
            regions[i].set_biome(Region::Biome::TROPICAL_FOREST);
        }
        else if(random==5){
            regions[i].set_biome(Region::Biome::TEMPERATE_FOREST);
        }
        else if(random==6){
            regions[i].set_biome(Region::Biome::GLACIER);
        }
        else if(random==7){
            regions[i].set_biome(Region::Biome::DESERT);
        }
        else{
            regions[i].set_biome(Region::Biome::ROCKY_WASTELAND);
        }
    }

    for(size_t i=0;i<leaders.size();i++){
        civilizations.push_back(Civilization());

        ///for(uint32_t attempt=0;attempt<20000;attempt++){
            cities.push_back(City(i));
            uint32_t city=cities.size()-1;
            civs[i].add_child_city(city);

            int x=rng.random_range(0,option_world_size_x*CHUNK_SIZE-BUILDING_SIZE);
            int y=rng.random_range(0,option_world_size_y*CHUNK_SIZE-BUILDING_SIZE);

            buildings.push_back(Building(city,x,y));
            cities[city].add_child_building(buildings.size()-1);

            people.push_back(Person(city,x,y));
            cities[city].add_child_person(people.size()-1);
        ///}
    }

    started=true;

    //Begin the network turn timer
    Network_Lockstep::start();
}

void Game::tick(){
    if(started){
    }
}

void Game::ai(){
    if(started){
    }
}

void Game::movement(){
    if(started){
    }
}

void Game::events(){
    if(started){
        Sound_Manager::set_listener(Game_Manager::camera.center_x(),Game_Manager::camera.center_y(),Game_Manager::camera_zoom);
    }
}

void Game::animate(){
    if(started){
    }
}

void Game::render(){
    if(started){
        uint32_t camera_x=(uint32_t)(Game_Manager::camera.x/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
        uint32_t camera_y=(uint32_t)(Game_Manager::camera.y/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
        uint32_t end_x=camera_x+(uint32_t)(Game_Manager::camera.w/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom))+2;
        uint32_t end_y=camera_y+(uint32_t)(Game_Manager::camera.h/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom))+2;

        //Render each onscreen chunk's ground
        for(uint32_t x=camera_x;x<end_x;x++){
            for(uint32_t y=camera_y;y<end_y;y++){
                if(x<option_world_width && y<option_world_height){
                    chunks[x][y].render_ground(x,y);
                }
            }
        }
    }
}

void Game::render_to_textures(){
    if(started){
        /**Rtt_Manager::set_render_target("example");
        ///Render something here
        Rtt_Manager::reset_render_target();*/
    }
}

void Game::update_background(){
    if(started){
    }
}

void Game::render_background(){
    if(started){
        Render::render_rectangle(0,0,Game_Window::width(),Game_Window::height(),1.0,"ui_black");
    }
}

bool Game::move_input_state(string direction){
    if(Object_Manager::game_command_state(direction)){
        return true;
    }

    if(direction=="up" && Object_Manager::game_command_state("move_stick_vertical")<-Engine_Data::controller_dead_zone){
        return true;
    }
    else if(direction=="down" && Object_Manager::game_command_state("move_stick_vertical")>Engine_Data::controller_dead_zone){
        return true;
    }
    else if(direction=="left" && Object_Manager::game_command_state("move_stick_horizontal")<-Engine_Data::controller_dead_zone){
        return true;
    }
    else if(direction=="right" && Object_Manager::game_command_state("move_stick_horizontal")>Engine_Data::controller_dead_zone){
        return true;
    }

    return false;
}
