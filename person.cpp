/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "person.h"
#include "game_constants.h"
#include "game.h"

#include <engine.h>
#include <game_manager.h>
#include <render.h>
#include <image_manager.h>

///QQQ includes
#include <object_manager.h>
#include <font.h>
#include <engine_strings.h>
#include <log.h>
///

using namespace std;

Person::Person(){
    parent_city=0;

    exists=true;

    health=get_health_max();
    hunger=Game_Constants::HUNGER_FULL;
}

Person::Person(uint32_t new_parent,const Collision_Rect<int32_t>& new_box){
    parent_city=new_parent;

    box=new_box;

    exists=true;

    health=get_health_max();
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

bool Person::is_inventory_space_low() const{
    uint32_t denominator=1000/Game_Constants::INVENTORY_ALMOST_FULL_THRESHOLD;

    if(!has_inventory_space((Game_Constants::INVENTORY_MAX*10)/denominator)){
        return true;
    }
    else{
        return false;
    }
}

bool Person::has_item_to_deposit() const{
    if(get_item_count()==1 && !has_food()){
        return true;
    }
    else if(get_item_count()>1){
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

bool Person::has_build_material() const{
    uint32_t tree_count=get_item_count(Inventory::Item_Type::TREE);

    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

    tree_count+=civilization.get_item_count(Inventory::Item_Type::TREE);

    if(tree_count>=Game_Constants::COST_BUILD){
        return true;
    }
    else{
        return false;
    }
}

bool Person::has_repair_material() const{
    uint32_t tree_count=get_item_count(Inventory::Item_Type::TREE);

    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

    tree_count+=civilization.get_item_count(Inventory::Item_Type::TREE);

    if(tree_count>=Game_Constants::COST_REPAIR){
        return true;
    }
    else{
        return false;
    }
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
    return health<=get_health_max()/4;
}

void Person::damage(int16_t attack,bool direct){
    if(is_alive()){
        int16_t damage_done=attack;
        if(!direct){
            damage_done-=get_defense();
        }

        if(damage_done<0){
            damage_done=0;
        }

        health-=damage_done;

        //If we were just killed
        if(!is_alive()){
            abandon_goal();
        }
    }
}

int16_t Person::get_health_max() const{
    return Game_Constants::PERSON_HEALTH_MAX;
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

bool Person::could_damage_person(const Person& person) const{
    if(get_attack()-person.get_defense()>0){
        return true;
    }
    else{
        return false;
    }
}

bool Person::could_damage_tile(const Tile& tile) const{
    if(get_attack()-tile.get_defense()>0){
        return true;
    }
    else{
        return false;
    }
}

bool Person::home_was_recently_captured() const{
    const City& city=Game::get_city(get_parent_city());

    return city.was_recently_captured();
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

            if(health>get_health_max()){
                health=get_health_max();
            }
        }
        else if(is_starving()){
            damage(Game_Constants::PERSON_HEALTH_CHANGE_RATE,true);
        }
    }
}

bool Person::is_friends_with_person(uint32_t person_index) const{
    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

    const Person& person=Game::get_person(person_index);

    return civilization.is_friends_with(person.get_parent_civilization());
}

bool Person::is_enemies_with_person(uint32_t person_index) const{
    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

    const Person& person=Game::get_person(person_index);

    return civilization.is_enemies_with(person.get_parent_civilization());
}

bool Person::is_neutral_towards_person(uint32_t person_index) const{
    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

    const Person& person=Game::get_person(person_index);

    return civilization.is_neutral_towards(person.get_parent_civilization());
}

bool Person::is_friends_with_tile(const Coords<uint32_t>& tile_coords) const{
    if(Game::tile_exists(tile_coords)){
        const Tile& tile=Game::get_tile(tile_coords);

        if(tile.is_building()){
            const Civilization& our_civilization=Game::get_civilization(get_parent_civilization());

            Tile::Type tile_type=tile.get_type();

            //If the tile's type is an unfinished building, its parent is a civilization
            if(tile_type==Tile::Type::BUILDING_UNFINISHED){
                return our_civilization.is_friends_with(tile.get_parent());
            }
            //If the tile's type is a city building, its parent is a city
            else{
                const City& city=Game::get_city(tile.get_parent());

                return our_civilization.is_friends_with(city.get_parent_civilization());
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

bool Person::is_enemies_with_tile(const Coords<uint32_t>& tile_coords) const{
    if(Game::tile_exists(tile_coords)){
        const Tile& tile=Game::get_tile(tile_coords);

        if(tile.is_building()){
            const Civilization& our_civilization=Game::get_civilization(get_parent_civilization());

            Tile::Type tile_type=tile.get_type();

            //If the tile's type is an unfinished building, its parent is a civilization
            if(tile_type==Tile::Type::BUILDING_UNFINISHED){
                return our_civilization.is_enemies_with(tile.get_parent());
            }
            //If the tile's type is a city building, its parent is a city
            else{
                const City& city=Game::get_city(tile.get_parent());

                return our_civilization.is_enemies_with(city.get_parent_civilization());
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

bool Person::is_neutral_towards_tile(const Coords<uint32_t>& tile_coords) const{
    if(Game::tile_exists(tile_coords)){
        const Tile& tile=Game::get_tile(tile_coords);

        if(tile.is_building()){
            const Civilization& our_civilization=Game::get_civilization(get_parent_civilization());

            Tile::Type tile_type=tile.get_type();

            //If the tile's type is an unfinished building, its parent is a civilization
            if(tile_type==Tile::Type::BUILDING_UNFINISHED){
                return our_civilization.is_neutral_towards(tile.get_parent());
            }
            //If the tile's type is a city building, its parent is a city
            else{
                const City& city=Game::get_city(tile.get_parent());

                return our_civilization.is_neutral_towards(city.get_parent_civilization());
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
    else if(goal.is_empty_inventory() && has_item_to_deposit()){
        const City& city=Game::get_city(get_parent_city());

        if(city.get_tile()==goal.get_coords_tiles()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_eat() && has_food()){
        return true;
    }
    else if(goal.is_eat_at_home()){
        const Civilization& civilization=Game::get_civilization(get_parent_civilization());
        const City& city=Game::get_city(get_parent_city());

        if(civilization.has_food() && city.get_tile()==goal.get_coords_tiles()){
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

        if(person.is_alive() && is_enemies_with_person(goal.get_person_index())){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_attack_building_melee() && Game::tile_exists(goal.get_coords_tiles())){
        const Tile& tile=Game::get_tile(goal.get_coords_tiles());

        if(tile.is_alive() && is_enemies_with_tile(goal.get_coords_tiles())){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_build() && Game::tile_exists(goal.get_coords_tiles())){
        const Tile& tile=Game::get_tile(goal.get_coords_tiles());

        //If the tile's type is an unfinished building, its parent is a civilization
        if(tile.is_alive() && tile.get_type()==Tile::Type::BUILDING_UNFINISHED && tile.get_parent()==get_parent_civilization() && has_build_material()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(goal.is_repair() && Game::tile_exists(goal.get_coords_tiles())){
        const Tile& tile=Game::get_tile(goal.get_coords_tiles());

        //If the tile's type is a city building, its parent is a city
        if(tile.is_alive() && tile.needs_repair() && tile.get_type()==Tile::Type::BUILDING_CITY && tile.get_parent()==get_parent_city() && has_repair_material()){
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
    return Game_Constants::INTERACTION_RANGE*Game_Constants::INTERACTION_RANGE;
}

uint64_t Person::get_goal_distance() const{
    Coords<int32_t> goal_coords=get_goal_coords();

    if(goal_coords.x>=0){
        return Int_Math::distance_between_points_no_sqrt(box.center_x(),box.center_y(),goal_coords.x,goal_coords.y);
    }
    else{
        return 0;
    }
}

Coords<int32_t> Person::get_goal_coords() const{
    //pixels
    int32_t goal_x=-1;
    int32_t goal_y=-1;

    if(goal.is_gather() || goal.is_empty_inventory() || goal.is_eat_at_home() || goal.is_forage() || goal.is_attack_building_melee() ||
       goal.is_build() || goal.is_repair()){
        Coords<uint32_t> tile_coords=goal.get_coords_tiles();

        goal_x=Tile::get_center_x(tile_coords.x,Tile::get_tile_type_size(goal.get_goal_tile_type()));
        goal_y=Tile::get_center_y(tile_coords.y,Tile::get_tile_type_size(goal.get_goal_tile_type()));
    }
    else if(goal.is_eat() || goal.is_retreat()){
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
    if(get_goal_distance()<=get_goal_range()){
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

bool Person::is_at_home() const{
    const City& city=Game::get_city(get_parent_city());

    if(Int_Math::distance_between_points_no_sqrt(box.center_x(),box.center_y(),city.get_center_x(),city.get_center_y())<=get_goal_range()){
        return true;
    }
    else{
        return false;
    }
}

void Person::notify_of_person_death(uint32_t index){
    if(goal.target_is_person() && goal.get_person_index()==index){
        abandon_goal();
    }
}

void Person::grab_some_food(){
    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

    if(civilization.has_food() && has_inventory_space() && !has_food()){
        add_item(Inventory::Item_Type::WHEAT,1);

        Game::remove_civilization_item(get_parent_civilization(),Inventory::Item_Type::WHEAT,1);
    }
}

void Person::abandon_goal(){
    if(goal.is_build()){
        Game::set_civilization_unfinished_building_flag(get_parent_civilization(),goal.get_coords_tiles(),false);
    }

    goal.clear_goal();
}

void Person::complete_goal(RNG& rng){
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

                if(item_count>0){
                    Game::add_civilization_item(get_parent_civilization(),item_type,item_count);
                    remove_item(item_type,item_count);
                }
            }

            grab_some_food();
        }
        else if(goal.is_eat()){
            eat();

            remove_item(Inventory::Item_Type::WHEAT,1);
        }
        else if(goal.is_eat_at_home()){
            eat();

            Game::remove_civilization_item(get_parent_civilization(),Inventory::Item_Type::WHEAT,1);

            grab_some_food();
        }
        else if(goal.is_forage()){
            eat();

            Game::kill_tile(goal.get_coords_tiles());
        }
        else if(goal.is_retreat()){
            if(is_at_home()){
                grab_some_food();
            }
        }
        else if(goal.is_attack_person_melee()){
            Game::damage_person(goal.get_person_index(),get_attack());

            const Person& person=Game::get_person(goal.get_person_index());

            //If we just killed the person
            if(!person.is_alive()){
                //Take all of their inventory items
                vector<Inventory::Item_Type> item_types=Inventory::get_item_types();

                for(size_t i=0;i<item_types.size();i++){
                    add_item(item_types[i],person.get_item_count(item_types[i]));
                }
            }
            else{
                if(rng.random_range(0,99)<Game_Constants::MAINTAIN_TARGET_CHANCE){
                    //Reset the counter and return early, so we don't abandon the goal down below
                    goal.reset_counter();
                    return;
                }
            }
        }
        else if(goal.is_attack_building_melee()){
            Game::damage_tile(goal.get_coords_tiles(),get_attack());

            const Tile& tile=Game::get_tile(goal.get_coords_tiles());

            //If we just killed the tile
            if(!tile.is_alive()){
                if(tile.get_type()==Tile::Type::BUILDING_CITY){
                    Game::handle_city_capture(goal.get_coords_tiles(),get_parent_civilization());
                }
            }
            else{
                if(rng.random_range(0,99)<Game_Constants::MAINTAIN_TARGET_CHANCE){
                    //Reset the counter and return early, so we don't abandon the goal down below
                    goal.reset_counter();
                    return;
                }
            }
        }
        else if(goal.is_build()){
            const Tile& tile=Game::get_tile(goal.get_coords_tiles());

            //If the tile's type is an unfinished building, its parent is a civilization
            Game::new_cities.push_back(City(tile.get_parent()));

            Game::new_cities.back().set_tile(goal.get_coords_tiles());

            uint32_t our_materials=get_item_count(Inventory::Item_Type::TREE);

            //If we can afford the cost from our inventory
            if(our_materials>=Game_Constants::COST_BUILD){
                remove_item(Inventory::Item_Type::TREE,Game_Constants::COST_BUILD);
            }
            else{
                remove_item(Inventory::Item_Type::TREE,our_materials);

                Game::remove_civilization_item(get_parent_civilization(),Inventory::Item_Type::TREE,Game_Constants::COST_BUILD-our_materials);
            }
        }
        else if(goal.is_repair()){
            Game::repair_tile(goal.get_coords_tiles());

            uint32_t our_materials=get_item_count(Inventory::Item_Type::TREE);

            //If we can afford the cost from our inventory
            if(our_materials>=Game_Constants::COST_REPAIR){
                remove_item(Inventory::Item_Type::TREE,Game_Constants::COST_REPAIR);
            }
            else{
                remove_item(Inventory::Item_Type::TREE,our_materials);

                Game::remove_civilization_item(get_parent_civilization(),Inventory::Item_Type::TREE,Game_Constants::COST_REPAIR-our_materials);
            }

            grab_some_food();
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
        Int_Vector acceleration=force/Game_Constants::PERSON_MASS;

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
        if(box.x+box.w>=Game::get_world_width()){
            box.x=Game::get_world_width()-box.w;
        }
        if(box.y+box.h>=Game::get_world_height()){
            box.y=Game::get_world_height()-box.h;
        }
    }
}

void Person::write_info_string(string& text) const{
    text+="Goal: ";

    text+=goal.get_type_string()+"\n";

    text+="\n";

    string hunger_state="";

    if(is_hungry()){
        hunger_state=" (hungry)";
    }
    else if(is_starving()){
        hunger_state=" (starving)";
    }

    text+="Health: "+Strings::num_to_string(health)+"/"+Strings::num_to_string(get_health_max())+hunger_state+"\n";

    text+="\n";

    text+="Inventory:\n";

    vector<Inventory::Item_Type> item_types=Inventory::get_item_types();

    for(size_t i=0;i<item_types.size();i++){
        text+=Inventory::get_item_type_string(item_types[i])+": "+Strings::num_to_string(get_item_count(item_types[i]))+"\n";
    }
}

void Person::render(bool selected) const{
    if(is_alive()){
        //pixels
        double x=box.x;
        double y=box.y;

        Collision_Rect<double> box_render(x,y,(double)box.w,(double)box.h);

        if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
            ///QQQ should render the person graphic

            const Civilization& civilization=Game::get_civilization(get_parent_civilization());

            string color=civilization.get_color();

            Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (double)box.w*Game_Manager::camera_zoom,(double)box.h*Game_Manager::camera_zoom,1.0,color);

            double percentage=(double)get_health()/(double)get_health_max();
            double max_bar_width=(double)box.w;
            double bar_width=max_bar_width*percentage;
            double bar_height=Game_Constants::RENDER_BAR_HEIGHT;
            double back_thickness=Game_Constants::RENDER_BAR_THICKNESS;

            string color_bar_health="health_normal";
            if(health_low()){
                color_bar_health="health_low";
            }

            Render::render_rectangle((x-back_thickness)*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                     (y-bar_height-back_thickness*4.0)*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (max_bar_width+back_thickness*2.0)*Game_Manager::camera_zoom,(bar_height+back_thickness*2.0)*Game_Manager::camera_zoom,1.0,"ui_black");
            Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                     (y-bar_height-back_thickness*3.0)*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     bar_width*Game_Manager::camera_zoom,bar_height*Game_Manager::camera_zoom,1.0,color_bar_health);

            if(is_hungry()){
                Render::render_texture((x+max_bar_width+back_thickness*2.0)*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                       (y-bar_height-back_thickness*4.0)*Game_Manager::camera_zoom-Game_Manager::camera.y,Image_Manager::get_image("icon_person_hungry"),1.0,
                                       Game_Manager::camera_zoom,Game_Manager::camera_zoom);
            }
            else if(is_starving()){
                Render::render_texture((x+max_bar_width+back_thickness*2.0)*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                       (y-bar_height-back_thickness*4.0)*Game_Manager::camera_zoom-Game_Manager::camera.y,Image_Manager::get_image("icon_person_starving"),1.0,
                                       Game_Manager::camera_zoom,Game_Manager::camera_zoom);
            }

            if(selected){
                Render::render_rectangle_empty((x-Game_Constants::RENDER_SELECTION_SIZE)*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                               (y-Game_Constants::RENDER_SELECTION_SIZE)*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                               ((double)box.w+Game_Constants::RENDER_SELECTION_SIZE*2.0)*Game_Manager::camera_zoom,
                                               ((double)box.h+Game_Constants::RENDER_SELECTION_SIZE*2.0)*Game_Manager::camera_zoom,
                                               1.0,"selection",Game_Constants::RENDER_SELECTION_BORDER*Game_Manager::camera_zoom);

                if(has_goal()){
                    Render::render_texture(x*Game_Manager::camera_zoom-Game_Manager::camera.x,(y-bar_height-back_thickness*4.0-16)*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                           Image_Manager::get_image("arrow"),1.0,Game_Manager::camera_zoom,Game_Manager::camera_zoom,get_angle_to_goal(),"selection");
                }
            }

            ///QQQ dev data
            /**Collision_Rect<int32_t> box_sight=get_sight_box();
            Render::render_rectangle((double)box_sight.x*Game_Manager::camera_zoom-Game_Manager::camera.x,
                                     (double)box_sight.y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                     (double)box_sight.w*Game_Manager::camera_zoom,(double)box_sight.h*Game_Manager::camera_zoom,0.25,"red");*/
            ///
        }
    }
}
