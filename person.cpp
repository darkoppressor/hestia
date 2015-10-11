/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "person.h"
#include "game_constants.h"
#include "game.h"

#include <engine.h>
#include <game_manager.h>
#include <render.h>

///QQQ
#include <object_manager.h>
#include <font.h>
#include <engine_strings.h>
#include <log.h>
///

using namespace std;

Person::Person(){
    parent_city=0;

    exists=true;

    health=Game_Constants::PERSON_HEALTH_MAX;
    hunger=Game_Constants::HUNGER_FULL;
}

Person::Person(uint32_t new_parent,const Collision_Rect<int32_t>& new_box){
    parent_city=new_parent;

    box=new_box;

    exists=true;

    health=Game_Constants::PERSON_HEALTH_MAX;
    hunger=Game_Constants::HUNGER_FULL;
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

Collision_Rect<int32_t> Person::get_box() const{
    return box;
}

Collision_Rect<int32_t> Person::get_sight_box() const{
    int32_t sight_range=Game_Constants::SIGHT_RANGE*(int32_t)Game_Constants::TILE_SIZE;

    return Collision_Rect<int32_t>(box.center_x()-sight_range,box.center_y()-sight_range,sight_range*2,sight_range*2);
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

bool Person::has_food() const{
    return get_item_count(Inventory::Item_Type::WHEAT)>0;
}

uint32_t Person::add_item(Inventory::Item_Type item_type,uint32_t amount){
    return inventory.add_item(item_type,amount);
}

void Person::remove_item(Inventory::Item_Type item_type,uint32_t amount){
    inventory.remove_item(item_type,amount);
}

int32_t Person::get_center_x() const{
    return box.center_x();
}

int32_t Person::get_center_y() const{
    return box.center_y();
}

uint32_t Person::get_chunk_x() const{
    return (uint32_t)box.center_x()/(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

uint32_t Person::get_chunk_y() const{
    return (uint32_t)box.center_y()/(Game_Constants::CHUNK_SIZE*Game_Constants::TILE_SIZE);
}

uint32_t Person::get_tile_x() const{
    return (uint32_t)box.center_x()/Game_Constants::TILE_SIZE;
}

uint32_t Person::get_tile_y() const{
    return (uint32_t)box.center_y()/Game_Constants::TILE_SIZE;
}

bool Person::get_exists() const{
    return exists;
}

void Person::set_exists(bool new_exists){
    exists=new_exists;
}

int32_t Person::get_max_speed() const{
    if(goal.is_combat()){
        return Game_Constants::PERSON_MAX_SPEED_COMBAT;
    }
    else{
        return Game_Constants::PERSON_MAX_SPEED;
    }
}

int32_t Person::get_move_force() const{
    if(goal.is_combat()){
        return Game_Constants::PERSON_MOVE_FORCE_COMBAT;
    }
    else{
        return Game_Constants::PERSON_MOVE_FORCE;
    }
}

bool Person::is_alive() const{
    return health>0;
}

bool Person::health_low() const{
    return health<=Game_Constants::PERSON_HEALTH_MAX/4;
}

void Person::damage(int16_t attack){
    int16_t damage_done=attack-get_defense();

    if(damage_done<0){
        damage_done=0;
    }

    health-=damage_done;
}

int16_t Person::get_health() const{
    return health;
}

int16_t Person::get_attack() const{
    return Game_Constants::PERSON_ATTACK;
}

int16_t Person::get_defense() const{
    return Game_Constants::PERSON_DEFENSE;
}

bool Person::is_full() const{
    return !is_hungry() && !is_starving();
}

bool Person::is_hungry() const{
    return hunger<=Game_Constants::HUNGER_HUNGRY && hunger>Game_Constants::HUNGER_STARVING;
}

bool Person::is_starving() const{
    return hunger<=Game_Constants::HUNGER_STARVING;
}

void Person::eat(){
    hunger=Game_Constants::HUNGER_FULL;
}

void Person::process_biology(){
    if(is_alive()){
        if(hunger>0){
            hunger--;
        }

        if(is_full()){
            health+=Game_Constants::PERSON_HEALTH_CHANGE_RATE;

            if(health>Game_Constants::PERSON_HEALTH_MAX){
                health=Game_Constants::PERSON_HEALTH_MAX;
            }
        }
        else if(is_starving()){
            health-=Game_Constants::PERSON_HEALTH_CHANGE_RATE;
        }
    }
}

bool Person::has_goal() const{
    return goal.is_in_progress();
}

bool Person::is_goal_valid() const{
    if(goal.is_gather() && has_inventory_space() && Game::tile_exists(goal.get_coords_tiles())){
        const Tile& tile=Game::get_tile(goal.get_coords_tiles());

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
    else if(goal.is_eat() && has_food()){
        return true;
    }
    else if(goal.is_eat_at_home()){
        const Civilization& civilization=Game::get_civilization(get_parent_civilization());

        if(civilization.has_food()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_forage() && Game::tile_exists(goal.get_coords_tiles())){
        const Tile& tile=Game::get_tile(goal.get_coords_tiles());

        if(tile.is_alive() && tile.is_food()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_retreat()){
        return true;
    }
    else if(goal.is_attack_person_melee()){
        const Person& person=Game::get_person(goal.get_person_index());

        if(person.is_alive()){
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

uint64_t Person::get_goal_range() const{
    uint64_t range=Game_Constants::INTERACTION_RANGE*Game_Constants::TILE_SIZE;

    return range*range;
}

Coords<int32_t> Person::get_goal_coords() const{
    //pixels
    int32_t goal_x=-1;
    int32_t goal_y=-1;

    if(goal.is_gather() || goal.is_forage()){
        Coords<uint32_t> tile_coords=goal.get_coords_tiles();

        goal_x=Tile::get_center_x(tile_coords.x,Tile::get_tile_type_size(goal.get_goal_tile_type()));
        goal_y=Tile::get_center_y(tile_coords.y,Tile::get_tile_type_size(goal.get_goal_tile_type()));
    }
    else if(goal.is_empty_inventory() || goal.is_eat() || goal.is_eat_at_home() || goal.is_retreat()){
        Coords<int32_t> goal_coords=goal.get_coords_pixels();

        goal_x=goal_coords.x;
        goal_y=goal_coords.y;
    }
    else if(goal.is_attack_person_melee()){
        const Person& person=Game::get_person(goal.get_person_index());

        goal_x=person.get_center_x();
        goal_y=person.get_center_y();
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

void Person::notify_of_person_death(uint32_t index){
    if(goal.target_is_person() && goal.get_person_index()==index){
        abandon_goal();
    }
}

void Person::abandon_goal(){
    goal.clear_goal();
}

void Person::complete_goal(){
    if(is_goal_valid()){
        if(goal.is_gather()){
            const Tile& tile=Game::get_tile(goal.get_coords_tiles());

            if(tile.get_type()==Tile::Type::WHEAT){
                add_item(Inventory::Item_Type::WHEAT,1);
            }
            else if(tile.get_type()==Tile::Type::TREE){
                add_item(Inventory::Item_Type::TREE,1);
            }

            Game::kill_tile(goal.get_coords_tiles());
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
        else if(goal.is_eat()){
            eat();

            remove_item(Inventory::Item_Type::WHEAT,1);
        }
        else if(goal.is_eat_at_home()){
            eat();

            Game::remove_civilization_item(get_parent_civilization(),Inventory::Item_Type::WHEAT,1);
        }
        else if(goal.is_forage()){
            eat();

            Game::kill_tile(goal.get_coords_tiles());
        }
        else if(goal.is_retreat()){
            //Nothing needs to be done after successfully retreating
        }
        else if(goal.is_attack_person_melee()){
            Game::damage_person(goal.get_person_index(),get_attack());
        }
    }

    abandon_goal();
}

void Person::brake(){
    Int_Vector brake_force(get_move_force()*2,velocity.direction+180);

    Int_Math::clamp_angle(brake_force.direction);

    if(brake_force.magnitude/Game_Constants::PERSON_MASS>velocity.magnitude){
        brake_force.magnitude=velocity.magnitude*Game_Constants::PERSON_MASS;
    }

    force+=brake_force;
}

void Person::accelerate(){
    if(is_alive()){
        acceleration=force/Game_Constants::PERSON_MASS;

        velocity+=acceleration;

        if(velocity.magnitude>get_max_speed()){
            velocity.magnitude=get_max_speed();
        }
        else if(velocity.magnitude<-get_max_speed()){
            velocity.magnitude=-get_max_speed();
        }

        force*=0;
    }
}

void Person::movement(){
    if(is_alive()){
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
}

void Person::render() const{
    if(is_alive()){
        double x=box.x;
        double y=box.y;

        Collision_Rect<double> box_render(x,y,(double)box.w,(double)box.h);

        if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
            ///QQQ - This is temporary
            const Civilization& civilization=Game::get_civilization(get_parent_civilization());

            string color=civilization.get_color();
            ///

            Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (double)box.w*Game_Manager::camera_zoom,(double)box.h*Game_Manager::camera_zoom,1.0,color);

            ///QQQ
            Collision_Rect<int32_t> box_sight=get_sight_box();
            Render::render_rectangle((double)box_sight.x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                     (double)box_sight.y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (double)box_sight.w*Game_Manager::camera_zoom,(double)box_sight.h*Game_Manager::camera_zoom,0.25,"red");
            Bitmap_Font* font=Object_Manager::get_font("small");
            string msg="Goal: ";
            if(goal.is_gather_wheat()){
                msg+="Gather wheat\n";
            }
            else if(goal.is_gather_tree()){
                msg+="Gather tree\n";
            }
            else if(goal.is_empty_inventory()){
                msg+="Empty inventory\n";
            }
            else if(goal.is_eat()){
                msg+="Eat\n";
            }
            else if(goal.is_eat_at_home()){
                msg+="Eat at home\n";
            }
            else if(goal.is_forage()){
                msg+="Forage\n";
            }
            else if(goal.is_retreat()){
                msg+="Retreat\n";
            }
            else if(goal.is_attack_person_melee()){
                msg+="Attack person melee\n";
            }
            else{
                msg+="None\n";
            }
            msg+="Health: "+Strings::num_to_string(health)+"/"+Strings::num_to_string(Game_Constants::PERSON_HEALTH_MAX)+"\n";
            msg+="Hunger: "+Strings::num_to_string((uint32_t)hunger)+"\n";
            msg+="Wheat: "+Strings::num_to_string(inventory.get_item_count(Inventory::Item_Type::WHEAT))+"\n";
            msg+="Tree: "+Strings::num_to_string(inventory.get_item_count(Inventory::Item_Type::TREE))+"\n";
            msg+="Friendlies: "+Strings::num_to_string(diag1)+"\n";
            msg+="Enemies: "+Strings::num_to_string(diag2)+"\n";
            msg+="Total: "+Strings::num_to_string(diag3)+"\n";
            ///msg+=Strings::num_to_string(goal.get_coords_tiles().x)+","+Strings::num_to_string(goal.get_coords_tiles().y);
            font->show(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,msg,"white");
            ///
        }
    }
}
