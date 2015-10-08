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
#include <utility>

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

uint32_t Person::get_parent_civilization() const{
    const City& city=Game::get_city(get_parent_city());

    return city.get_parent_civilization();
}

uint32_t Person::get_item_count() const{
    return inventory.get_item_count();
}

uint32_t Person::get_item_count(Inventory::Item_Type item_type) const{
    return inventory.get_item_count(item_type);
}

bool Person::has_inventory_space(uint32_t amount) const{
    if(get_item_count()>=Game_Constants::INVENTORY_MAX){
        return false;
    }

    if(Game_Constants::INVENTORY_MAX-get_item_count()>=amount){
        return true;
    }
    else{
        return false;
    }
}

uint32_t Person::add_item(Inventory::Item_Type item_type,uint32_t amount){
    return inventory.add_item(item_type,amount);
}

void Person::remove_item(Inventory::Item_Type item_type,uint32_t amount){
    inventory.remove_item(item_type,amount);
}

bool Person::has_goal() const{
    return goal.is_in_progress();
}

bool Person::is_goal_valid() const{
    if(goal.is_gather() && has_inventory_space() && Game::tile_exists(goal.coords)){
        const Tile& tile=Game::get_tile(goal.coords);

        if(tile.is_alive() && tile.is_gatherable()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_empty_inventory() && get_item_count()>0){
        return true;
    }
    else{
        return false;
    }
}

uint32_t Person::get_chunk_x() const{
    return (uint32_t)box.center_x()/(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

uint32_t Person::get_chunk_y() const{
    return (uint32_t)box.center_y()/(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

uint64_t Person::get_goal_range() const{
    uint64_t range=Game_Constants::INTERACTION_RANGE*Game_Constants::TILE_SIZE;

    return range*range;
}

Coords<int32_t> Person::get_goal_coords() const{
    //pixels
    int32_t goal_x=-1;
    int32_t goal_y=-1;

    if(goal.is_gather()){
        goal_x=Tile::get_center_x(goal.coords.x,Tile::get_tile_type_size(goal.get_gather_tile_type()));
        goal_y=Tile::get_center_y(goal.coords.y,Tile::get_tile_type_size(goal.get_gather_tile_type()));
    }
    else if(goal.is_empty_inventory()){
        goal_x=(int32_t)goal.coords.x;
        goal_y=(int32_t)goal.coords.y;
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

void Person::find_gather_tile(RNG& rng){
    const City& city=Game::get_city(parent_city);

    if(city.get_gather_zone_tile_count(goal.get_gather_tile_type())>0){
        vector<Coords<uint32_t>> chunk_coords=city.get_gather_zone_chunk_coords();

        Sorting::shuffle(rng,chunk_coords);

        Coords<uint32_t> our_chunk_coords(get_chunk_x(),get_chunk_y());

        for(size_t i=0;i<chunk_coords.size();i++){
            if(chunk_coords[i]==our_chunk_coords){
                if(i>0){
                    swap(chunk_coords[i],chunk_coords[0]);
                }

                break;
            }
        }

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

                        if(tile.is_alive() && tile.get_type()==goal.get_gather_tile_type()){
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
    }

    //If we did not find a valid gather tile
    abandon_goal();
}

void Person::abandon_goal(){
    goal.set_type(AI_Goal::Type::NONE);

    goal.coords.x=0;
    goal.coords.y=0;
}

void Person::complete_goal(){
    if(is_goal_valid()){
        if(goal.is_gather()){
            const Tile& tile=Game::get_tile(goal.coords);

            if(tile.get_type()==Tile::Type::WHEAT){
                add_item(Inventory::Item_Type::WHEAT,1);
            }
            else if(tile.get_type()==Tile::Type::TREE){
                add_item(Inventory::Item_Type::TREE,1);
            }

            Game::kill_tile(goal.coords);
        }
        else if(goal.is_empty_inventory()){
            vector<Inventory::Item_Type> item_types=Inventory::get_item_types();

            for(size_t i=0;i<item_types.size();i++){
                Inventory::Item_Type item_type=item_types[i];
                uint32_t item_count=get_item_count(item_type);
                uint32_t item_minimum=0;

                //Keep 1 piece of wheat if possible
                if(item_type==Inventory::Item_Type::WHEAT){
                    item_minimum=1;
                }

                if(item_count>item_minimum){
                    uint32_t item_exchange=item_count-item_minimum;

                    Game::add_civilization_item(get_parent_civilization(),item_type,item_exchange);
                    remove_item(item_type,item_exchange);
                }
            }
        }
    }

    abandon_goal();
}

bool Person::allowed_to_select_ai_goal(uint32_t frame,uint32_t index) const{
    if((frame+(index%Engine::UPDATE_RATE))%Game_Constants::AI_GOAL_SELECTION_PERIOD==0){
        return true;
    }
    else{
        return false;
    }
}

void Person::ai(RNG& rng,uint32_t frame,uint32_t index){
    if(!has_goal() && allowed_to_select_ai_goal(frame,index)){
        if(has_inventory_space()){
            goal.set_type(AI_Goal::Type::GATHER_WHEAT);
        }
        else{
            goal.set_type(AI_Goal::Type::EMPTY_INVENTORY);
        }

        if(goal.is_gather()){
            find_gather_tile(rng);
        }
        else if(goal.is_empty_inventory()){
            const City& city=Game::get_city(get_parent_city());

            goal.coords.x=city.get_center_x();
            goal.coords.y=city.get_center_y();
        }
    }

    if(has_goal()){
        if(goal_within_range()){
            brake();

            if(goal.is_first_count() && !is_goal_valid()){
                abandon_goal();
            }
            else{
                if(goal.countdown()){
                    complete_goal();
                }
            }
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
        /**Bitmap_Font* font=Object_Manager::get_font("small");
        string msg="";
        if(goal.type==AI_Goal::Type::GATHER_WHEAT){
            msg="Gather wheat\n";
        }
        else if(goal.type==AI_Goal::Type::GATHER_TREE){
            msg="Gather tree\n";
        }
        else if(goal.type==AI_Goal::Type::EMPTY_INVENTORY){
            msg="Empty inventory\n";
        }
        msg+=Strings::num_to_string(goal.coords.x)+","+Strings::num_to_string(goal.coords.y);
        font->show(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,msg,"white");*/
        ///
    }
}
