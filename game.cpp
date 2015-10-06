/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_constants.h"
#include "int_math.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <network_lockstep.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <network_engine.h>
#include <collision.h>

using namespace std;

bool Game::started=false;

uint32_t Game::option_rng_seed=0;
uint32_t Game::option_world_width=0;
uint32_t Game::option_world_height=0;
uint32_t Game::option_region_min=0;
uint32_t Game::option_region_max=0;
uint32_t Game::option_initial_tile_growth=0;
uint32_t Game::option_max_leaders=0;

RNG Game::rng;

Calendar Game::calendar;

vector<Region> Game::regions;
vector<vector<Chunk>> Game::chunks;
vector<Leader> Game::leaders;
vector<Civilization> Game::civilizations;
vector<City> Game::cities;
vector<Person> Game::people;
map<Coords<uint32_t>,Tile,Game::tile_compare> Game::tiles;

void Game::clear_world(){
    started=false;

    calendar.reset();

    RNG rng_seeder;
    option_rng_seed=rng_seeder.random_range(0,UINT32_MAX);

    option_world_width=5;
    option_world_height=5;
    option_region_min=4;
    option_region_max=8;
    option_initial_tile_growth=720;

    option_max_leaders=8;

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
        if(i<player_count){
            leaders.push_back(Leader(i));
        }
        else{
            leaders.push_back(Leader());
        }

        leaders.back().set_color(Color(rng_color.random_range(0,255),rng_color.random_range(0,255),rng_color.random_range(0,255),255));
    }
}

void Game::generate_world(){
    ///QQQ - Note that I have removed the usual call to clear_world()
    ///This would be a problem if the standard singleplayer start function was used

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

    //pixels
    //We square this value, so it can be directly compared to distances returned by Int_Math::distance_between_points_no_sqrt
    uint64_t desired_distance_between_cities=((get_world_width()+get_world_height())/2)/4;
    desired_distance_between_cities*=desired_distance_between_cities;

    for(uint32_t leader=0;leader<leaders.size();leader++){
        civilizations.push_back(Civilization(leader));
        uint32_t civilization=civilizations.size()-1;
        leaders.back().set_civilization(civilization);

        cities.push_back(City(civilization));
        uint32_t city=cities.size()-1;
        civilizations.back().add_city(city);

        const uint32_t max_attempts=10000;

        for(uint32_t attempts=0;;attempts++){
            //tiles
            uint32_t tile_x=rng.random_range(0,get_world_width_tiles()-Game_Constants::BUILDING_SIZE);
            uint32_t tile_y=rng.random_range(0,get_world_height_tiles()-Game_Constants::BUILDING_SIZE);
            Coords<uint32_t> tile_coords(tile_x,tile_y);
            Coords<int32_t> coords(Tile::get_x(tile_x)+Tile::get_tile_type_size(Tile::Type::BUILDING_CITY)/2,Tile::get_y(tile_y)+Tile::get_tile_type_size(Tile::Type::BUILDING_CITY)/2);

            if(tile_coords_are_valid(Tile::Type::BUILDING_CITY,tile_coords)){
                if(attempts>=max_attempts || distance_to_nearest_city(coords)>=desired_distance_between_cities){
                    tiles.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(tile_coords),std::forward_as_tuple(city,Tile::Type::BUILDING_CITY));
                    cities.back().set_tile(tile_coords);

                    break;
                }
            }
        }

        //pixels
        int32_t city_spawn_zone_left=cities.back().get_center_x()-cities.back().get_size()*4;
        int32_t city_spawn_zone_top=cities.back().get_center_y()-cities.back().get_size()*4;
        int32_t city_spawn_zone_right=city_spawn_zone_left+cities.back().get_size()*8;
        int32_t city_spawn_zone_bottom=city_spawn_zone_top+cities.back().get_size()*8;

        if(city_spawn_zone_left<0){
            city_spawn_zone_left=0;
        }
        if(city_spawn_zone_top<0){
            city_spawn_zone_top=0;
        }
        if(city_spawn_zone_right+Game_Constants::PERSON_SIZE>=get_world_width()){
            city_spawn_zone_right=get_world_width()-1-Game_Constants::PERSON_SIZE;
        }
        if(city_spawn_zone_bottom+Game_Constants::PERSON_SIZE>=get_world_height()){
            city_spawn_zone_bottom=get_world_height()-1-Game_Constants::PERSON_SIZE;
        }

        for(uint32_t person=0;person<Game_Constants::CITY_POPULATION_START;person++){
            int32_t x=(int32_t)rng.random_range((uint32_t)city_spawn_zone_left,(uint32_t)city_spawn_zone_right);
            int32_t y=(int32_t)rng.random_range((uint32_t)city_spawn_zone_top,(uint32_t)city_spawn_zone_bottom);

            people.push_back(Person(city,Collision_Rect<int32_t>(x,y,Game_Constants::PERSON_SIZE,Game_Constants::PERSON_SIZE)));
            cities.back().add_person(people.size()-1);
        }
    }

    for(size_t i=0;i<regions.size();i++){
        for(uint32_t attempts=0;attempts<option_initial_tile_growth;attempts++){
            regions[i].tile_growth(rng);
        }
    }

    //If we are a player, center the camera on our starting city
    int player_number=Network_Engine::get_our_player_number();
    if(player_number>=0){
        int32_t leader=get_player_leader(player_number);

        if(leader>=0){
            uint32_t civilization=leaders[leader].get_civilization();

            vector<uint32_t> civ_cities=civilizations[civilization].get_cities();

            City* city_ptr=&cities[civ_cities[0]];

            Game_Manager::center_camera(Collision_Rect<double>((double)city_ptr->get_center_x(),(double)city_ptr->get_center_y(),0.0,0.0));
        }
    }

    started=true;

    //Begin the network turn timer
    Network_Lockstep::start();
}

int32_t Game::get_player_leader(int player_number){
    if(player_number>=0 && player_number<Network_Engine::get_player_count()){
        for(size_t i=0;i<leaders.size();i++){
            if(leaders[i].is_player_controlled() && leaders[i].get_parent_player()==player_number){
                return (int32_t)i;
            }
        }
    }

    return -1;
}

uint32_t Game::get_world_width(){
    return option_world_width*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE;
}

uint32_t Game::get_world_height(){
    return option_world_height*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE;
}

uint32_t Game::get_world_width_tiles(){
    return option_world_width*Game_Constants::CHUNK_SIZE;
}

uint32_t Game::get_world_height_tiles(){
    return option_world_height*Game_Constants::CHUNK_SIZE;
}

bool Game::tile_exists(const Coords<uint32_t>& tile_coords){
    if(tiles.count(tile_coords)){
        return true;
    }
    else{
        return false;
    }
}

bool Game::tile_coords_are_valid(Tile::Type type,const Coords<uint32_t>& tile_coords){
    //tiles
    uint32_t tile_size=1;
    if(Tile::tile_type_is_building(type)){
        tile_size=Game_Constants::BUILDING_SIZE;
    }

    if(tile_coords.x+(tile_size-1)>=get_world_width_tiles() || tile_coords.y+(tile_size-1)>=get_world_height_tiles()){
        return false;
    }

    uint32_t x=tile_coords.x;
    uint32_t y=tile_coords.y;

    if(x>=Game_Constants::BUILDING_SIZE-1){
        x-=Game_Constants::BUILDING_SIZE-1;
    }
    else{
        x=0;
    }
    if(y>=Game_Constants::BUILDING_SIZE-1){
        y-=Game_Constants::BUILDING_SIZE-1;
    }
    else{
        y=0;
    }

    uint32_t x_end=tile_coords.x+tile_size-1;
    uint32_t y_end=tile_coords.y+tile_size-1;

    for(;x<=x_end;x++){
        for(;y<=y_end;y++){
            if(tile_exists(Coords<uint32_t>(x,y))){
                return false;
            }
        }
    }

    return true;
}

uint64_t Game::distance_to_nearest_city(const Coords<int32_t>& coords){
    bool no_city_found=true;
    uint64_t nearest=0;

    for(size_t i=0;i<cities.size();i++){
        uint64_t distance=Int_Math::distance_between_points_no_sqrt(coords.x,coords.y,cities[i].get_center_x(),cities[i].get_center_y());

        if(no_city_found || distance<nearest){
            no_city_found=false;

            nearest=distance;
        }
    }

    return nearest;
}

void Game::tick(){
    if(started){
        Calendar::Change change=calendar.increment();

        //If the day changed
        if(change!=Calendar::Change::NONE){
            for(size_t i=0;i<regions.size();i++){
                regions[i].tile_growth(rng);
            }

            //If the week changed
            if(change!=Calendar::Change::DAY){
                ///Do weekly stuff

                //If the month changed
                if(change!=Calendar::Change::WEEK){
                    ///Do monthly stuff

                    //If the year changed
                    if(change!=Calendar::Change::MONTH){
                        ///Do yearly stuff
                    }
                }
            }
        }
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
        uint32_t camera_chunk_x=(uint32_t)(Game_Manager::camera.x/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
        uint32_t camera_chunk_y=(uint32_t)(Game_Manager::camera.y/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
        uint32_t end_chunk_x=camera_chunk_x+(uint32_t)(Game_Manager::camera.w/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom))+2;
        uint32_t end_chunk_y=camera_chunk_y+(uint32_t)(Game_Manager::camera.h/((double)Game_Constants::CHUNK_SIZE*(double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom))+2;

        //Render each on-screen chunk's ground
        for(uint32_t x=camera_chunk_x;x<end_chunk_x;x++){
            for(uint32_t y=camera_chunk_y;y<end_chunk_y;y++){
                if(x<option_world_width && y<option_world_height){
                    chunks[x][y].render_ground(x,y);
                }
            }
        }

        uint32_t camera_tile_x=(uint32_t)(Game_Manager::camera.x/((double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
        uint32_t camera_tile_y=(uint32_t)(Game_Manager::camera.y/((double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
        uint32_t end_tile_x=camera_tile_x+(uint32_t)(Game_Manager::camera.w/((double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom))+2;
        uint32_t end_tile_y=camera_tile_y+(uint32_t)(Game_Manager::camera.h/((double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom))+2;

        //Render each on-screen tile
        for(uint32_t x=camera_tile_x;x<end_tile_x;x++){
            for(uint32_t y=camera_tile_y;y<end_tile_y;y++){
                if(x<get_world_width_tiles() && y<get_world_height_tiles()){
                    Coords<uint32_t> tile_coords(x,y);

                    if(tile_exists(tile_coords)){
                        tiles.at(tile_coords).render(x,y);
                    }
                }
            }
        }

        for(size_t i=0;i<people.size();i++){
            people[i].render();
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
