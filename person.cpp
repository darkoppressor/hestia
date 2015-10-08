/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "person.h"
#include "game_constants.h"
#include "game.h"

#include <render.h>
#include <game_manager.h>
#include <engine.h>
#include <sorting.h>

///QQQ
#include <object_manager.h>
#include <font.h>
#include <engine_strings.h>
///

#include <vector>

using namespace std;

Person::Person(){
    parent_city=0;
}

Person::Person(uint32_t new_parent,const Collision_Rect<int32_t>& new_box){
    parent_city=new_parent;

    box=new_box;
}

uint32_t Person::get_parent_city() const{
    return parent_city;
}

void Person::set_parent_city(uint32_t new_parent){
    parent_city=new_parent;
}

bool Person::has_goal() const{
    return goal.type!=AI_Goal::Type::NONE;
}

uint64_t Person::get_goal_range() const{
    if(goal.type==AI_Goal::Type::GATHER_WHEAT || goal.type==AI_Goal::Type::GATHER_TREE){
        uint64_t range=Game_Constants::GATHER_RANGE*Game_Constants::TILE_SIZE;

        return range*range;
    }
    else{
        return 0;
    }
}

Coords<int32_t> Person::get_goal_coords() const{
    //pixels
    int32_t goal_x=-1;
    int32_t goal_y=-1;

    if(goal.type==AI_Goal::Type::GATHER_WHEAT || goal.type==AI_Goal::Type::GATHER_TREE){
        if(Game::tile_exists(goal.coords)){
            const Tile& tile=Game::get_tile(goal.coords);

            goal_x=tile.get_center_x(goal.coords.x);
            goal_y=tile.get_center_y(goal.coords.y);
        }
    }

    return Coords<int32_t>(goal_x,goal_y);
}

bool Person::goal_within_range() const{
    uint64_t goal_range=get_goal_range();

    Coords<int32_t> goal_coords=get_goal_coords();

    if(goal_coords.x>=0 && Int_Math::distance_between_points_no_sqrt(box.x,box.y,goal_coords.x,goal_coords.y)<=goal_range){
        return true;
    }
    else{
        return false;
    }
}

int32_t Person::get_angle_to_goal() const{
    Coords<int32_t> goal_coords=get_goal_coords();

    if(goal_coords.x>=0){
        return Int_Collision::get_angle_to_rect(Collision_Rect<int32_t>(box.x,box.y,0,0),Collision_Rect<int32_t>(goal_coords.x,goal_coords.y,0,0));
    }
    else{
        return 0;
    }
}

Collision_Rect<uint32_t> Person::get_gather_zone() const{
    const City& city=Game::get_city(parent_city);

    //chunks
    uint32_t gather_zone_x=city.get_chunk_x();
    uint32_t gather_zone_y=city.get_chunk_y();
    uint32_t gather_zone_width=Game_Constants::GATHER_ZONE_RANGE*2;
    uint32_t gather_zone_height=Game_Constants::GATHER_ZONE_RANGE*2;

    if(gather_zone_x>=Game_Constants::GATHER_ZONE_RANGE){
        gather_zone_x-=Game_Constants::GATHER_ZONE_RANGE;
    }
    else{
        gather_zone_x=0;
    }

    if(gather_zone_y>=Game_Constants::GATHER_ZONE_RANGE){
        gather_zone_y-=Game_Constants::GATHER_ZONE_RANGE;
    }
    else{
        gather_zone_y=0;
    }

    if(gather_zone_x+gather_zone_width>=Game::option_world_width){
        gather_zone_width=Game::option_world_width-1-gather_zone_x;
    }
    if(gather_zone_y+gather_zone_height>=Game::option_world_height){
        gather_zone_height=Game::option_world_height-1-gather_zone_y;
    }

    return Collision_Rect<uint32_t>(gather_zone_x,gather_zone_y,gather_zone_width,gather_zone_height);
}

void Person::find_gather_tile(RNG& rng){
    Tile::Type tile_type_desired=Tile::Type::WHEAT;
    if(goal.type==AI_Goal::Type::GATHER_TREE){
        tile_type_desired=Tile::Type::TREE;
    }

    Collision_Rect<uint32_t> gather_zone=get_gather_zone();

    //A list of chunk coordinates within the gather zone
    vector<Coords<uint32_t>> chunk_coords;

    for(uint32_t x=gather_zone.x;x<gather_zone.x+gather_zone.w;x++){
        for(uint32_t y=gather_zone.y;y<gather_zone.y+gather_zone.h;y++){
            chunk_coords.push_back(Coords<uint32_t>(x,y));
        }
    }

    Sorting::shuffle(rng,chunk_coords);

    for(size_t i=0;i<chunk_coords.size();i++){
        uint32_t tile_x=chunk_coords[i].x*Game_Constants::CHUNK_SIZE;
        uint32_t tile_y=chunk_coords[i].y*Game_Constants::CHUNK_SIZE;

        //A list of valid tile coordinates within the chunk
        vector<Coords<uint32_t>> tile_coords;

        for(uint32_t x=tile_x;x<tile_x+Game_Constants::CHUNK_SIZE;x++){
            for(uint32_t y=tile_y;y<tile_y+Game_Constants::CHUNK_SIZE;y++){
                Coords<uint32_t> tile_check_coords(x,y);

                if(Game::tile_exists(tile_check_coords)){
                    const Tile& tile=Game::get_tile(tile_check_coords);

                    if(tile.get_type()==tile_type_desired){
                        tile_coords.push_back(tile_check_coords);
                    }
                }
            }
        }

        if(tile_coords.size()>0){
            uint32_t random_tile=rng.random_range(0,tile_coords.size()-1);

            goal.coords.x=tile_coords[random_tile].x;
            goal.coords.y=tile_coords[random_tile].y;

            return;
        }
    }

    //If we did not find a valid gather tile
    goal.type=AI_Goal::Type::NONE;
}

void Person::ai(RNG& rng){
    if(!has_goal()){
        goal.type=AI_Goal::Type::GATHER_WHEAT;

        if(goal.type==AI_Goal::Type::GATHER_WHEAT || goal.type==AI_Goal::Type::GATHER_TREE){
            find_gather_tile(rng);
        }
    }

    if(has_goal()){
        if(goal_within_range()){
            brake();

            ///
        }
        else{
            force+=Int_Vector(Game_Constants::PERSON_MOVE_FORCE,get_angle_to_goal());
        }
    }
    else{
        brake();
    }
}

void Person::brake(){
    Int_Vector brake_force(Game_Constants::PERSON_MOVE_FORCE,velocity.direction+180);

    Int_Math::clamp_angle(brake_force.direction);

    if(brake_force.magnitude/Game_Constants::PERSON_MASS>velocity.magnitude){
        brake_force.magnitude=velocity.magnitude*Game_Constants::PERSON_MASS;
    }

    force+=brake_force;
}

void Person::accelerate(){
    acceleration=force/Game_Constants::PERSON_MASS;

    velocity+=acceleration;

    if(velocity.magnitude>Game_Constants::PERSON_MAX_SPEED){
        velocity.magnitude=Game_Constants::PERSON_MAX_SPEED;
    }
    else if(velocity.magnitude<-Game_Constants::PERSON_MAX_SPEED){
        velocity.magnitude=-Game_Constants::PERSON_MAX_SPEED;
    }

    force*=0;
}

void Person::movement(){
    Int_Vector_Components vc=velocity.get_components();

    box.x+=vc.a/(int32_t)Engine::UPDATE_RATE;
    box.y+=vc.b/(int32_t)Engine::UPDATE_RATE;

    if(box.x<0){
        box.x=0;
    }
    if(box.y<0){
        box.y=0;
    }
    if(uint32_t(box.x+box.w)>=Game::get_world_width()){
        box.x=(int32_t)Game::get_world_width()-box.w;
    }
    if(uint32_t(box.y+box.h)>=Game::get_world_height()){
        box.y=(int32_t)Game::get_world_height()-box.h;
    }
}

void Person::render() const{
    double x=box.x;
    double y=box.y;

    Collision_Rect<double> box_render(x,y,(double)box.w,(double)box.h);

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                 (double)box.w*Game_Manager::camera_zoom,(double)box.h*Game_Manager::camera_zoom,1.0,"red");

        ///QQQ
        ///Bitmap_Font* font=Object_Manager::get_font("small");
        ///font->show(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,Strings::num_to_string(goal.coords.x)+","+Strings::num_to_string(goal.coords.y),"white");
        ///
    }
}
