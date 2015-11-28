/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_constants.h"
#include "int_math.h"
#include "network_game.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <network_lockstep.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <network_engine.h>
#include <collision.h>
#include <log.h>
#include <engine_strings.h>
#include <engine.h>
#include <window_manager.h>
#include <sorting.h>

#include <boost/crc.hpp>

using namespace std;

Game_Over::Game_Over(){
    reset();
}

void Game_Over::reset(){
    victory_condition=Victory_Condition::NONE;

    winning_leader=0;
}

void Game_Over::update(Victory_Condition new_victory_condition,uint32_t new_winning_leader){
    victory_condition=new_victory_condition;

    winning_leader=new_winning_leader;
}

Game_City_Distance::Game_City_Distance(uint32_t new_index,uint64_t new_distance){
    index=new_index;

    distance=new_distance;
}

bool Game_City_Distance::operator<=(const Game_City_Distance& game_city_distance) const{
    return distance<=game_city_distance.distance;
}

Game_Order::Type Game::order=Game_Order::Type::NONE;

vector<Game_Order> Game::server_game_orders;
vector<Game_Order> Game::client_game_orders;

Game_Selection Game::selection;

vector<Region> Game::regions;
vector<vector<Chunk>> Game::chunks;
vector<Leader> Game::leaders;
vector<Civilization> Game::civilizations;
vector<City> Game::cities;
vector<Person> Game::people;
map<Coords<uint32_t>,Tile,Game::tile_compare> Game::tiles;

queue<uint32_t> Game::dead_cities;
queue<uint32_t> Game::dead_people;

Quadtree<int32_t,uint32_t> Game::quadtree;

Minimap Game::minimap;

Game_Over Game::game_over;

void Game::add_remove_objects(){
    for(size_t i=0;i<people.size();i++){
        if(people[i].get_exists() && !people[i].is_alive()){
            uint32_t person_index=(uint32_t)i;

            uint32_t parent_city=people[i].get_parent_city();

            cities[parent_city].remove_person(person_index);

            for(size_t n=0;n<people.size();n++){
                if(n!=i && people[n].is_alive()){
                    people[n].notify_of_person_death(person_index);
                }
            }

            people[i].set_exists(false);

            dead_people.push(person_index);

            if(selection.type==Game_Selection::Type::PERSON && selection.index==person_index){
                clear_selection();
            }
        }
    }

    for(auto it=tiles.cbegin();it!=tiles.cend();){
        if(!it->second.is_alive()){
            //Let the parent chunk know it has one less tile in it
            chunks[Tile::get_chunk_x(it->first.x)][Tile::get_chunk_y(it->first.y)].decrement_tile_count(it->second.get_type());

            //If the tile's type is an unfinished building, its parent is a civilization
            if(it->second.get_type()==Tile::Type::BUILDING_UNFINISHED){
                uint32_t parent_civilization=it->second.get_parent();

                civilizations[parent_civilization].remove_unfinished_building(Coords<uint32_t>(it->first.x,it->first.y));

                if(selection.type==Game_Selection::Type::UNFINISHED_BUILDING && selection.tile_coordinates==it->first){
                    clear_selection();
                }
            }

            it=tiles.erase(it);
        }
        else{
            it++;
        }
    }

    for(size_t i=0;i<new_cities.size();i++){
        uint32_t city_index=0;

        if(dead_cities.empty()){
            cities.push_back(new_cities[i]);

            city_index=cities.size()-1;
        }
        else{
            city_index=dead_cities.front();

            dead_cities.pop();

            cities[city_index]=new_cities[i];
        }

        uint32_t parent_civilization=cities[city_index].get_parent_civilization();

        civilizations[parent_civilization].add_city(city_index);

        Coords<uint32_t> tile_coords=cities[city_index].get_tile();

        civilizations[parent_civilization].remove_unfinished_building(tile_coords);

        tiles.at(tile_coords).set_type(Tile::Type::BUILDING_CITY);

        //If the tile's type is a city building, its parent is a city
        tiles.at(tile_coords).set_parent(city_index);
    }
    new_cities.clear();

    for(size_t i=0;i<new_people.size();i++){
        uint32_t person_index=0;

        if(dead_people.empty()){
            people.push_back(new_people[i]);

            person_index=people.size()-1;
        }
        else{
            person_index=dead_people.front();

            dead_people.pop();

            people[person_index]=new_people[i];
        }

        uint32_t parent_city=people[person_index].get_parent_city();

        cities[parent_city].add_person(person_index);
    }
    new_people.clear();

    for(const auto& it : new_tiles){
        tiles.emplace(it.first,it.second);

        //Let the parent chunk know it has a new tile in it
        chunks[Tile::get_chunk_x(it.first.x)][Tile::get_chunk_y(it.first.y)].increment_tile_count(it.second.get_type());

        //If the tile's type is an unfinished building, its parent is a civilization
        if(it.second.get_type()==Tile::Type::BUILDING_UNFINISHED){
            uint32_t parent_civilization=it.second.get_parent();

            civilizations[parent_civilization].add_unfinished_building(Coords<uint32_t>(it.first.x,it.first.y));
        }
    }
    new_tiles.clear();
}

void Game::generate_minimap(){
    minimap.generate_surface(320,180);
}

void Game::update_minimap(){
    minimap.update();
}

void Game::clear_minimap(){
    minimap.clear_map();
}

bool Game::started=false;

uint32_t Game::frame=0;

string Game::edge_scroll_state="none";
double Game::edge_scroll_speed=0.0;

bool Game::camera_drag_main=false;
bool Game::camera_drag_minimap=false;

uint32_t Game::option_rng_seed=0;
uint32_t Game::option_world_width=0;
uint32_t Game::option_world_height=0;
uint32_t Game::option_region_min=0;
uint32_t Game::option_region_max=0;
uint32_t Game::option_initial_tile_growth=0;
bool Game::option_vc_conquest=0;
uint32_t Game::option_max_leaders=0;

RNG Game::rng;

Calendar Game::calendar;

vector<City> Game::new_cities;
vector<Person> Game::new_people;
map<Coords<uint32_t>,Tile,Game::tile_compare> Game::new_tiles;

void Game::clear_world(){
    started=false;

    frame=0;

    clear_order();

    clear_server_game_orders();
    clear_client_game_orders();

    clear_selection();

    calendar.reset();

    RNG rng_seeder;
    option_rng_seed=rng_seeder.random_range(0,UINT32_MAX);

    option_world_width=20;
    option_world_height=20;
    option_region_min=4;
    option_region_max=8;
    option_initial_tile_growth=1440;

    option_vc_conquest=true;

    //chunks
    uint32_t MAX_WORLD_SIZE=(uint32_t)INT32_MAX/(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);

    if(option_world_width>MAX_WORLD_SIZE){
        option_world_width=MAX_WORLD_SIZE;
    }
    if(option_world_height>MAX_WORLD_SIZE){
        option_world_height=MAX_WORLD_SIZE;
    }

    option_max_leaders=8;

    for(size_t i=0;i<leaders.size();i++){
        Object_Manager::remove_color(Leader::get_color((uint32_t)i));
    }

    regions.clear();
    chunks.clear();
    leaders.clear();
    civilizations.clear();
    cities.clear();
    people.clear();
    tiles.clear();

    dead_cities={};
    dead_people={};

    quadtree.clear_tree();

    clear_minimap();

    game_over.reset();

    new_cities.clear();
    new_people.clear();
    new_tiles.clear();
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

    for(size_t i=0;i<leaders.size();i++){
        leaders[i].initialize_diplomacy_states((uint32_t)i,(uint32_t)leaders.size());
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
    uint64_t desired_distance_between_cities=(uint64_t)((get_world_width()+get_world_height())/2)/4;
    desired_distance_between_cities*=desired_distance_between_cities;

    for(uint32_t leader=0;leader<leaders.size();leader++){
        civilizations.push_back(Civilization(leader));
        uint32_t civilization=civilizations.size()-1;
        leaders[leader].set_civilization(civilization);

        cities.push_back(City(civilization));
        uint32_t city=cities.size()-1;
        civilizations.back().add_city(city);

        const uint32_t max_attempts=10000;

        for(uint32_t attempts=0;;attempts++){
            //tiles
            uint32_t tile_x=rng.random_range(0,(uint32_t)(get_world_width_tiles()-(int32_t)Game_Constants::BUILDING_SIZE));
            uint32_t tile_y=rng.random_range(0,(uint32_t)(get_world_height_tiles()-(int32_t)Game_Constants::BUILDING_SIZE));
            Coords<uint32_t> tile_coords(tile_x,tile_y);
            Coords<int32_t> coords(Tile::get_x(tile_x)+Tile::get_tile_type_size(Tile::Type::BUILDING_CITY)/2,Tile::get_y(tile_y)+Tile::get_tile_type_size(Tile::Type::BUILDING_CITY)/2);

            if(tile_coords_are_valid(Tile::Type::BUILDING_CITY,tile_coords)){
                if(attempts>=max_attempts || distance_to_nearest_city(coords)>=desired_distance_between_cities){
                    new_tiles.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(tile_coords),std::forward_as_tuple(city,Tile::Type::BUILDING_CITY));
                    cities.back().set_tile(tile_coords);

                    break;
                }
            }
        }

        Collision_Rect<int32_t> box_city_spawn_zone=cities.back().get_spawn_zone();

        for(uint32_t person=0;person<Game_Constants::CITY_POPULATION_START;person++){
            int32_t x=(int32_t)rng.random_range((uint32_t)box_city_spawn_zone.x,(uint32_t)(box_city_spawn_zone.x+box_city_spawn_zone.w));
            int32_t y=(int32_t)rng.random_range((uint32_t)box_city_spawn_zone.y,(uint32_t)(box_city_spawn_zone.y+box_city_spawn_zone.h));

            new_people.push_back(Person(city,Collision_Rect<int32_t>(x,y,Game_Constants::PERSON_SIZE,Game_Constants::PERSON_SIZE)));
        }
    }

    for(size_t i=0;i<regions.size();i++){
        for(uint32_t attempts=0;attempts<option_initial_tile_growth;attempts++){
            regions[i].tile_growth(rng);
        }
    }

    quadtree.setup(10,5,0,Collision_Rect<int32_t>(0,0,get_world_width(),get_world_height()));

    for(size_t i=0;i<leaders.size();i++){
        Object_Manager::add_color(Leader::get_color((uint32_t)i),leaders[i].get_color());
    }

    generate_minimap();
    update_minimap();

    //If we are a player, center the camera on our starting city
    int32_t leader=get_our_leader();
    if(leader>=0){
        uint32_t civilization=leaders[leader].get_civilization();

        vector<uint32_t> civ_cities=civilizations[civilization].get_cities();

        City* city_ptr=&cities[civ_cities[0]];

        Game_Manager::center_camera(Collision_Rect<double>((double)city_ptr->get_center_x(),(double)city_ptr->get_center_y(),0.0,0.0));
    }

    started=true;

    //Begin the network turn timer
    Network_Lockstep::start();
}

uint32_t Game::get_people_count(){
    return people.size();
}

uint32_t Game::get_city_count(){
    return cities.size();
}

const map<Coords<uint32_t>,Tile,Game::tile_compare>& Game::get_tiles(){
    return tiles;
}

const Region& Game::get_region(uint32_t index){
    if(index<regions.size()){
        return regions[index];
    }
    else{
        Log::add_error("Error accessing region '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Chunk& Game::get_chunk(const Coords<uint32_t>& coords){
    if(coords.x<chunks.size() && coords.y<chunks[coords.x].size()){
        return chunks[coords.x][coords.y];
    }
    else{
        Log::add_error("Error accessing chunk '"+Strings::num_to_string(coords.x)+","+Strings::num_to_string(coords.y)+"'");

        Engine::quit();
    }
}

const Leader& Game::get_leader(uint32_t index){
    if(index<leaders.size()){
        return leaders[index];
    }
    else{
        Log::add_error("Error accessing leader '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Civilization& Game::get_civilization(uint32_t index){
    if(index<civilizations.size()){
        return civilizations[index];
    }
    else{
        Log::add_error("Error accessing civilization '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const City& Game::get_city(uint32_t index){
    if(index<cities.size()){
        return cities[index];
    }
    else{
        Log::add_error("Error accessing city '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Person& Game::get_person(uint32_t index){
    if(index<people.size()){
        return people[index];
    }
    else{
        Log::add_error("Error accessing person '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Tile& Game::get_tile(const Coords<uint32_t>& coords){
    if(tile_exists(coords)){
        return tiles.at(coords);
    }
    else{
        Log::add_error("Error accessing tile '"+Strings::num_to_string(coords.x)+","+Strings::num_to_string(coords.y)+"'");

        Engine::quit();
    }
}

uint32_t Game::get_leader_count(){
    return leaders.size();
}

void Game::add_leader(bool player_controlled,uint32_t parent_player,const Color& color,const vector<Leader::Diplomacy_State>& diplomacy_states){
    if(!started){
        if(player_controlled){
            leaders.push_back(Leader(parent_player));
        }
        else{
            leaders.push_back(Leader());
        }

        leaders.back().set_color(color);

        leaders.back().set_diplomacy_states(diplomacy_states);
    }
}

int32_t Game::get_player_leader(int player_number){
    if(player_number>=0 && (uint32_t)player_number<Network_Engine::get_player_count()){
        for(size_t i=0;i<leaders.size();i++){
            if(leaders[i].is_player_controlled() && leaders[i].get_parent_player()==(uint32_t)player_number){
                return (int32_t)i;
            }
        }
    }

    return -1;
}

int32_t Game::get_our_leader(){
    return get_player_leader(Network_Engine::get_our_player_number());
}

Game_Over Game::get_game_over(){
    return game_over;
}

int32_t Game::get_world_width(){
    return int32_t(option_world_width*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

int32_t Game::get_world_height(){
    return int32_t(option_world_height*Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
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

    //Tile is invalid if outside of the world
    if(tile_coords.x+(tile_size-1)>=get_world_width_tiles() || tile_coords.y+(tile_size-1)>=get_world_height_tiles()){
        return false;
    }

    //Tile is invalid if it contains an existing tile, or if it is contained in an existing tile

    uint32_t x_start=tile_coords.x;
    uint32_t y_start=tile_coords.y;

    if(x_start>Game_Constants::BUILDING_SIZE){
        x_start-=Game_Constants::BUILDING_SIZE;
    }
    else{
        x_start=0;
    }

    if(y_start>Game_Constants::BUILDING_SIZE){
        y_start-=Game_Constants::BUILDING_SIZE;
    }
    else{
        y_start=0;
    }

    for(uint32_t x=x_start;x<tile_coords.x+tile_size;x++){
        for(uint32_t y=y_start;y<tile_coords.y+tile_size;y++){
            Coords<uint32_t> check_coords(x,y);

            if(tile_exists(check_coords)){
                //tiles
                uint32_t check_size=1;
                if(tiles.at(check_coords).is_building()){
                    check_size=Game_Constants::BUILDING_SIZE;
                }

                Collision_Rect<uint32_t> box_tile_1(tile_coords.x,tile_coords.y,tile_size,tile_size);
                Collision_Rect<uint32_t> box_tile_2(x,y,check_size-1,check_size-1);

                if(Collision::check_rect(box_tile_1,box_tile_2)){
                    return false;
                }
            }
            else if(new_tiles.count(check_coords)){
                //tiles
                uint32_t check_size=1;
                if(new_tiles.at(check_coords).is_building()){
                    check_size=Game_Constants::BUILDING_SIZE;
                }

                Collision_Rect<uint32_t> box_tile_1(tile_coords.x,tile_coords.y,tile_size,tile_size);
                Collision_Rect<uint32_t> box_tile_2(x,y,check_size-1,check_size-1);

                if(Collision::check_rect(box_tile_1,box_tile_2)){
                    return false;
                }
            }
        }
    }

    return true;
}

void Game::kill_tile(const Coords<uint32_t>& tile_coords){
    if(tile_exists(tile_coords)){
        tiles.at(tile_coords).die();
    }
}

Collision_Rect<uint32_t> Game::get_city_spacing_area(const Coords<uint32_t>& tile_coords){
    uint32_t city_spacing=Game_Constants::CITY_SPACING;

    uint32_t tile_start_x=tile_coords.x;
    uint32_t tile_start_y=tile_coords.y;

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

    if(tile_end_x>=get_world_width_tiles()){
        tile_end_x=get_world_width_tiles()-1;
    }
    if(tile_end_y>=get_world_height_tiles()){
        tile_end_y=get_world_height_tiles()-1;
    }

    return Collision_Rect<uint32_t>(tile_start_x,tile_start_y,tile_end_x-tile_start_x,tile_end_y-tile_start_y);
}

void Game::clear_new_city_area(const Coords<uint32_t>& tile_coords){
    if(selection.type==Game_Selection::Type::UNFINISHED_BUILDING && selection.tile_coordinates==tile_coords){
        clear_selection();
    }

    Collision_Rect<uint32_t> city_spacing_area=get_city_spacing_area(tile_coords);

    for(uint32_t x=city_spacing_area.x;x<=city_spacing_area.x+city_spacing_area.w;x++){
        for(uint32_t y=city_spacing_area.y;y<=city_spacing_area.y+city_spacing_area.h;y++){
            Coords<uint32_t> coords(x,y);

            if(coords!=tile_coords && tile_exists(coords)){
                const Tile& tile=get_tile(coords);

                if(tile.is_alive() && tile.get_type()==Tile::Type::BUILDING_UNFINISHED){
                    kill_tile(coords);
                }
            }
        }
    }
}

uint32_t Game::add_civilization_item(uint32_t index,Inventory::Item_Type item_type,uint32_t amount){
    if(index<civilizations.size()){
        return civilizations[index].add_item(item_type,amount);
    }
    else{
        return 0;
    }
}

void Game::remove_civilization_item(uint32_t index,Inventory::Item_Type item_type,uint32_t amount){
    if(index<civilizations.size()){
        civilizations[index].remove_item(item_type,amount);
    }
}

void Game::set_civilization_unfinished_building_flag(uint32_t index,Coords<uint32_t> tile_coords,bool new_flag){
    if(index<civilizations.size()){
        civilizations[index].set_unfinished_building_flag(tile_coords,new_flag);
    }
}

void Game::clear_civilization_unfinished_buildings(uint32_t index){
    if(index<civilizations.size()){
        vector<Coords<uint32_t>> unfinished_buildings=civilizations[index].get_unfinished_buildings();

        for(size_t i=0;i<unfinished_buildings.size();i++){
            Coords<uint32_t> coords=unfinished_buildings[i];

            if(tile_exists(coords)){
                const Tile& tile=get_tile(coords);

                if(tile.is_alive() && tile.get_type()==Tile::Type::BUILDING_UNFINISHED){
                    kill_tile(coords);
                }
            }
        }
    }
}

void Game::defeat_civilization(uint32_t index){
    clear_civilization_unfinished_buildings(index);

    if(option_vc_conquest){
        uint32_t undefeated_civs=0;

        for(size_t i=0;i<civilizations.size();i++){
            if(!civilizations[i].is_defeated()){
                undefeated_civs++;
            }
        }

        //If only one civilization remains undefeated
        if(undefeated_civs==1){
            for(size_t i=0;i<civilizations.size();i++){
                if(!civilizations[i].is_defeated()){
                    game_over.update(Game_Over::Victory_Condition::CONQUEST,civilizations[i].get_parent_leader());

                    break;
                }
            }

            Window_Manager::get_window("game_over")->toggle_on(true,true);
        }
    }
}

void Game::damage_person(uint32_t index,int16_t attack){
    if(index<people.size()){
        people[index].damage(attack);
    }
}

void Game::damage_tile(const Coords<uint32_t>& tile_coords,int16_t attack){
    if(tile_exists(tile_coords)){
        tiles.at(tile_coords).damage(attack);
    }
}

void Game::repair_tile(const Coords<uint32_t>& tile_coords){
    if(tile_exists(tile_coords)){
        tiles.at(tile_coords).repair();
    }
}

void Game::handle_city_capture(const Coords<uint32_t>& tile_coords,uint32_t capturing_civilization_index){
    tiles.at(tile_coords).capture_heal();

    for(size_t i=0;i<people.size();i++){
        Person& person=people[i];

        if(person.is_alive()){
            person.abandon_goal();
        }
    }

    //If the tile's type is a city building, its parent is a city
    uint32_t city_index=tiles.at(tile_coords).get_parent();

    uint32_t losing_civilization_index=cities[city_index].get_parent_civilization();

    civilizations[losing_civilization_index].remove_city(city_index);

    civilizations[capturing_civilization_index].add_city(city_index);

    cities[city_index].set_parent_civilization(capturing_civilization_index);

    cities[city_index].set_just_captured();

    //If this city was the losing civilization's last city
    if(civilizations[losing_civilization_index].is_defeated()){
        defeat_civilization(losing_civilization_index);
    }

    if(selection.type==Game_Selection::Type::CITY && selection.index==city_index){
        clear_selection();
    }
}

void Game::repopulate_city(uint32_t city_index){
    City& passed_city=cities[city_index];

    const Civilization& civilization=get_civilization(passed_city.get_parent_civilization());

    vector<uint32_t> city_list=civilization.get_cities();

    //If the passed city's civilization has more cities than just the passed city
    if(city_list.size()>1 && !passed_city.has_maximum_population()){
        int32_t city_x=passed_city.get_center_x();
        int32_t city_y=passed_city.get_center_y();

        //List of cities in passed city's civilization, paired with their distance to the passed city
        //The passed city is not included in this list
        vector<Game_City_Distance> city_distance_list;

        for(size_t i=0;i<city_list.size();i++){
            if(city_list[i]!=city_index){
                const City& city=get_city(city_list[i]);

                if(city.get_exists()){
                    city_distance_list.push_back(Game_City_Distance(city_list[i],Int_Math::distance_between_points_no_sqrt(city_x,city_y,city.get_center_x(),city.get_center_y())));
                }
            }
        }

        if(city_distance_list.size()>0){
            Sorting::quick_sort(city_distance_list);

            for(size_t i=city_distance_list.size()-1;;){
                City& city=cities[city_distance_list[i].index];

                if(city.has_excess_population()){
                    uint32_t excess_population=city.get_excess_population();

                    uint32_t desired_population=Game_Constants::CITY_POPULATION_MAX-passed_city.get_population();

                    uint32_t population_to_move=0;

                    if(excess_population>=desired_population){
                        population_to_move=desired_population;
                    }
                    else{
                        population_to_move=excess_population;
                    }

                    vector<uint32_t> city_people=city.get_people();

                    for(size_t n=0,people_moved=0;n<city_people.size() && people_moved<population_to_move;n++){
                        uint32_t person_index=city_people[n];

                        Person& person=people[person_index];

                        if(person.is_alive()){
                            person.set_parent_city(city_index);

                            city.remove_person(person_index);

                            passed_city.add_person(person_index);

                            people_moved++;
                        }
                    }

                    desired_population=Game_Constants::CITY_POPULATION_MAX-passed_city.get_population();

                    if(desired_population==0){
                        break;
                    }
                }

                if(i==0){
                    break;
                }
                else{
                    i--;
                }
            }
        }
    }
}

void Game::abandon_city(uint32_t city_index){
    City& passed_city=cities[city_index];

    Civilization& civilization=civilizations[passed_city.get_parent_civilization()];

    vector<uint32_t> city_list=civilization.get_cities();

    //If the passed city's civilization has more cities than just the passed city
    if(city_list.size()>1){
        vector<uint32_t> city_people=passed_city.get_people();

        while(city_people.size()>0){
            for(size_t i=0;i<city_list.size();i++){
                if(city_list[i]!=city_index){
                    uint32_t person_index=city_people[0];

                    Person& person=people[person_index];

                    if(person.is_alive()){
                        City& city=cities[city_list[i]];

                        person.set_parent_city(city_list[i]);

                        passed_city.remove_person(person_index);

                        city.add_person(person_index);
                    }

                    city_people.erase(city_people.begin());

                    if(city_people.size()==0){
                        break;
                    }
                }
            }
        }

        kill_tile(passed_city.get_tile());

        civilization.remove_city(city_index);

        passed_city.set_exists(false);

        dead_cities.push(city_index);

        if(selection.type==Game_Selection::Type::CITY && selection.index==city_index){
            clear_selection();
        }
    }
}

vector<Game_City_Distance> Game::get_nearest_city(const Coords<int32_t>& coords){
    vector<Game_City_Distance> city;

    for(size_t i=0;i<cities.size();i++){
        if(cities[i].get_exists()){
            uint64_t distance=Int_Math::distance_between_points_no_sqrt(coords.x,coords.y,cities[i].get_center_x(),cities[i].get_center_y());

            if(city.size()==0){
                city.push_back(Game_City_Distance((uint32_t)i,distance));
            }
            else if(distance<city[0].distance){
                city[0].index=(uint32_t)i;
                city[0].distance=distance;
            }
        }
    }

    return city;
}

uint64_t Game::distance_to_nearest_city(const Coords<int32_t>& coords){
    vector<Game_City_Distance> nearest_city=get_nearest_city(coords);

    if(nearest_city.size()>0){
        return nearest_city[0].distance;
    }
    else{
        return 0;
    }
}

Coords<int32_t> Game::get_mouse_coords_pixels(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    //pixels
    double mouse_world_x=(Game_Manager::camera.x+(double)mouse_x)/Game_Manager::camera_zoom;
    double mouse_world_y=(Game_Manager::camera.y+(double)mouse_y)/Game_Manager::camera_zoom;

    return Coords<int32_t>((int32_t)mouse_world_x,(int32_t)mouse_world_y);
}

Coords<uint32_t> Game::get_mouse_coords_tiles(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    //pixels
    double mouse_world_x=Game_Manager::camera.x+(double)mouse_x;
    double mouse_world_y=Game_Manager::camera.y+(double)mouse_y;

    //tiles
    uint32_t mouse_tile_x=(uint32_t)(mouse_world_x/((double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));
    uint32_t mouse_tile_y=(uint32_t)(mouse_world_y/((double)Game_Constants::TILE_SIZE*Game_Manager::camera_zoom));

    return Coords<uint32_t>(mouse_tile_x,mouse_tile_y);
}

Game_Selection Game::get_selection(){
    return selection;
}

void Game::set_selection(Game_Selection::Type type,uint32_t index){
    selection.type=type;
    selection.index=index;

    close_selection_windows();

    open_selection_window();
}

void Game::set_selection(Game_Selection::Type type,const Coords<uint32_t>& tile_coords){
    selection.type=type;
    selection.tile_coordinates=tile_coords;

    close_selection_windows();

    open_selection_window();
}

void Game::toggle_selection_follow(){
    if(selection.follow){
        selection.follow=false;
    }
    else{
        if(selection.type==Game_Selection::Type::PERSON){
            selection.follow=true;
        }
    }
}

void Game::clear_selection(){
    selection.clear_type();

    //Ensure that the selection windows are all closed
    close_selection_windows();
}

void Game::open_selection_window(){
    if(selection.type==Game_Selection::Type::PERSON){
        Window_Manager::get_window("game_person")->toggle_on(true,true);
    }
    else if(selection.type==Game_Selection::Type::CITY){
        Window_Manager::get_window("game_city")->toggle_on(true,true);
    }
    else if(selection.type==Game_Selection::Type::UNFINISHED_BUILDING){
        Window_Manager::get_window("game_unfinished_building")->toggle_on(true,true);
    }
    else if(selection.type==Game_Selection::Type::CIVILIZATION){
        Window_Manager::get_window("game_civilization")->toggle_on(true,true);
    }
}

void Game::close_selection_windows(){
    Window_Manager::get_window("game_person")->toggle_on(true,false);
    Window_Manager::get_window("game_city")->toggle_on(true,false);
    Window_Manager::get_window("game_unfinished_building")->toggle_on(true,false);
    Window_Manager::get_window("game_civilization")->toggle_on(true,false);
}

Game_Order::Type Game::get_order(){
    return order;
}

bool Game::has_order(){
    return order!=Game_Order::Type::NONE;
}

void Game::set_order(Game_Order::Type new_order){
    order=new_order;
}

void Game::clear_order(){
    order=Game_Order::Type::NONE;
}

void Game::issue_order(const Game_Order& new_order){
    if(new_order.is_valid()){
        if(Network_Engine::status=="server"){
            server_game_orders.push_back(new_order);
        }
        else if(Network_Engine::status=="client"){
            Network_Game::send_client_game_order(new_order);
        }

        clear_order();
    }
}

vector<Game_Order> Game::get_server_game_orders(){
    return server_game_orders;
}

void Game::add_server_game_order(const Game_Order& new_order){
    server_game_orders.push_back(new_order);
}

void Game::clear_server_game_orders(){
    server_game_orders.clear();
}

vector<Game_Order> Game::get_client_game_orders(){
    return client_game_orders;
}

void Game::add_client_game_order(const Game_Order& new_order){
    client_game_orders.push_back(new_order);
}

void Game::clear_client_game_orders(){
    client_game_orders.clear();
}

void Game::execute_client_game_orders(){
    for(size_t i=0;i<client_game_orders.size();i++){
        client_game_orders[i].execute();
    }

    clear_client_game_orders();
}

bool Game::is_mouse_over_minimap(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    Collision_Rect<double> box_mouse((double)mouse_x,(double)mouse_y,1.0,1.0);
    Collision_Rect<double> box_minimap=minimap.get_box();

    if(Collision::check_rect(box_mouse,box_minimap)){
        return true;
    }
    else{
        return false;
    }
}

void Game::center_camera_on_minimap_position(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    Collision_Rect<double> box_mouse((double)mouse_x,(double)mouse_y,1.0,1.0);
    Collision_Rect<double> box_minimap=minimap.get_box();

    Collision_Rect<double> box_position(Math::abs(box_minimap.x-box_mouse.x)/(box_minimap.w/(double)get_world_width()),
                           Math::abs(box_minimap.y-box_mouse.y)/(box_minimap.h/(double)get_world_height()),0.0,0.0);

    if(box_mouse.x<box_minimap.x){
        box_position.x=0.0;
    }

    if(box_mouse.y<box_minimap.y){
        box_position.y=0.0;
    }

    Game_Manager::center_camera(box_position);
}

void Game::tick(){
    if(started){
        Calendar::Change change=calendar.increment();

        //If the day changed
        if(change!=Calendar::Change::NONE){
            for(size_t i=0;i<regions.size();i++){
                regions[i].tile_growth(rng);
            }

            for(size_t i=0;i<cities.size();i++){
                cities[i].breed((uint32_t)i,rng);
                cities[i].capture_cooldown();
            }

            for(size_t i=0;i<people.size();i++){
                people[i].process_biology();
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
        quadtree.clear_tree();
        for(size_t i=0;i<people.size();i++){
            if(people[i].is_alive()){
                quadtree.insert_object(people[i].get_box(),(uint32_t)i);
            }
        }

        for(size_t i=0;i<people.size();i++){
            people[i].ai(rng,quadtree,frame,(uint32_t)i);
        }
    }
}

void Game::movement(){
    if(started){
        for(size_t i=0;i<people.size();i++){
            people[i].accelerate();
        }

        for(size_t i=0;i<people.size();i++){
            people[i].movement();
        }
    }
}

void Game::events(){
    if(started){
        Sound_Manager::set_listener(Game_Manager::camera.center_x(),Game_Manager::camera.center_y(),Game_Manager::camera_zoom);

        execute_client_game_orders();

        add_remove_objects();

        ///QQQ move into separate thread
        update_minimap();

        for(size_t i=0;i<civilizations.size();i++){
            civilizations[i].update_needs(frame,(uint32_t)i);
        }

        for(size_t i=0;i<cities.size();i++){
            cities[i].update_gather_zone(frame,(uint32_t)i);
        }

        if(++frame==Engine::UPDATE_RATE){
            frame=0;
        }
    }
}

void Game::animate(){
    if(started){
        for(size_t i=0;i<people.size();i++){
            people[i].animate();
        }
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
                if(x<(uint32_t)get_world_width_tiles() && y<(uint32_t)get_world_height_tiles()){
                    Coords<uint32_t> tile_coords(x,y);

                    if(tile_exists(tile_coords)){
                        bool selected=false;

                        if(selection.type==Game_Selection::Type::CITY){
                            if(tiles.at(tile_coords).is_alive() && tiles.at(tile_coords).get_type()==Tile::Type::BUILDING_CITY){
                                //If the tile is of type BUILDING_CITY, its parent is a City
                                if(selection.index==tiles.at(tile_coords).get_parent()){
                                    selected=true;
                                }
                            }
                        }
                        else if(selection.type==Game_Selection::Type::UNFINISHED_BUILDING){
                            if(selection.tile_coordinates==tile_coords){
                                if(tiles.at(tile_coords).is_alive() && tiles.at(tile_coords).get_type()==Tile::Type::BUILDING_UNFINISHED){
                                    selected=true;
                                }
                            }
                        }

                        tiles.at(tile_coords).render(x,y,selected);
                    }
                }
            }
        }

        for(size_t i=0;i<people.size();i++){
            bool selected=false;

            if(selection.type==Game_Selection::Type::PERSON && selection.index==(uint32_t)i){
                selected=true;
            }

            people[i].render(selected);
        }

        minimap.render();

        int32_t leader=get_our_leader();
        if(leader>=0){
            Game_Order current_order(order,get_mouse_coords_tiles(),(uint32_t)leader);
            current_order.render();
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

uint32_t Game::get_checksum(){
    vector<uint32_t> data;

    data.push_back(option_rng_seed);
    data.push_back(option_world_width);
    data.push_back(option_world_height);
    data.push_back(option_region_min);
    data.push_back(option_region_max);
    data.push_back(option_initial_tile_growth);
    data.push_back((uint32_t)option_vc_conquest);

    data.push_back(rng.random_range(0,UINT32_MAX));

    for(size_t i=0;i<regions.size();i++){
        regions[i].add_checksum_data(data);
    }

    for(size_t x=0;x<chunks.size();x++){
        for(size_t y=0;y<chunks[x].size();y++){
            chunks[x][y].add_checksum_data(data);
        }
    }

    for(size_t i=0;i<leaders.size();i++){
        leaders[i].add_checksum_data(data);
    }

    for(size_t i=0;i<civilizations.size();i++){
        civilizations[i].add_checksum_data(data);
    }

    for(size_t i=0;i<cities.size();i++){
        cities[i].add_checksum_data(data);
    }

    for(size_t i=0;i<people.size();i++){
        people[i].add_checksum_data(data);
    }

    for(const auto& it : tiles){
        it.second.add_checksum_data(data);
    }

    data.push_back(dead_cities.size());
    data.push_back(dead_people.size());

    for(size_t i=0;i<new_cities.size();i++){
        new_cities[i].add_checksum_data(data);
    }

    for(size_t i=0;i<new_people.size();i++){
        new_people[i].add_checksum_data(data);
    }

    for(const auto& it : new_tiles){
        it.second.add_checksum_data(data);
    }

    boost::crc_32_type checksum;

    checksum.process_bytes(data.data(),data.size()*4);

    return (uint32_t)checksum.checksum();
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

bool Game::edge_scroll_input_state(string direction){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    int window_width=0;
    int window_height=0;
    Game_Window::get_renderer_output_size(&window_width,&window_height);

    if(direction=="up" && mouse_y<=1){
        return true;
    }
    else if(direction=="down" && mouse_y>=window_height-2){
        return true;
    }
    else if(direction=="left" && mouse_x<=1){
        return true;
    }
    else if(direction=="right" && mouse_x>=window_width-2){
        return true;
    }

    return false;
}
