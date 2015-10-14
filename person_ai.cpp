/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "person.h"
#include "game.h"
#include "game_constants.h"

#include <engine.h>
#include <sorting.h>

#include <utility>
#include <unordered_set>

using namespace std;

void Person::put_our_city_first(vector<uint32_t>& cities) const{
    uint32_t our_city=get_parent_city();

    for(size_t i=0;i<cities.size();i++){
        if(cities[i]==our_city){
            if(i>0){
                swap(cities[i],cities[0]);
            }

            break;
        }
    }
}

void Person::put_our_chunk_first(vector<Coords<uint32_t>>& chunk_coords) const{
    Coords<uint32_t> our_chunk_coords(get_chunk_x(),get_chunk_y());

    for(size_t i=0;i<chunk_coords.size();i++){
        if(chunk_coords[i]==our_chunk_coords){
            if(i>0){
                swap(chunk_coords[i],chunk_coords[0]);
            }

            break;
        }
    }
}

void Person::filter_zone_chunks(vector<Coords<uint32_t>>& chunk_coords,const vector<Tile::Type>& desired_tile_types) const{
    for(size_t i=0;i<chunk_coords.size();){
        const Chunk& chunk=Game::get_chunk(chunk_coords[i]);

        bool chunk_has_desired_tile=false;

        for(size_t n=0;n<desired_tile_types.size();n++){
            if(chunk.get_tile_count(desired_tile_types[n])>0){
                chunk_has_desired_tile=true;

                break;
            }
        }

        if(!chunk_has_desired_tile){
            chunk_coords.erase(chunk_coords.begin()+i);
        }
        else{
            i++;
        }
    }
}

bool Person::forage_zone_has_food(const vector<Coords<uint32_t>>& chunk_coords) const{
    for(size_t i=0;i<chunk_coords.size();i++){
        const Chunk& chunk=Game::get_chunk(chunk_coords[i]);

        if(chunk.has_food()){
            return true;
        }
    }

    return false;
}

void Person::find_tile(RNG& rng,const vector<Coords<uint32_t>>& chunk_coords){
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

                    if(tile.is_alive() && tile.get_type()==goal.get_goal_tile_type()){
                        tile_coords.push_back(tile_check_coords);
                    }
                }
            }
        }

        if(tile_coords.size()>0){
            uint32_t random_tile=rng.random_range(0,tile_coords.size()-1);

            goal.set_coords_tiles(Coords<uint32_t>(tile_coords[random_tile].x,tile_coords[random_tile].y));

            return;
        }
    }

    //If we did not find a valid tile
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

void Person::consider_ignoring(vector<AI_Choice>& choices) const{
    if(has_goal()){
        choices.push_back(AI_Choice(AI_Goal::Type::NONE,Game_Constants::PRIORITY_IGNORE));
    }
}

void Person::consider_gathering(vector<AI_Choice>& choices) const{
    if(goal.gather_can_interrupt()){
        if(has_inventory_space()){
            const City& city=Game::get_city(parent_city);

            if(city.get_gather_zone_tile_count(Tile::Type::WHEAT)>0){
                choices.push_back(AI_Choice(AI_Goal::Type::GATHER_WHEAT,Game_Constants::PRIORITY_GATHER));
            }

            if(city.get_gather_zone_tile_count(Tile::Type::TREE)>0){
                choices.push_back(AI_Choice(AI_Goal::Type::GATHER_TREE,Game_Constants::PRIORITY_GATHER));
            }
        }
    }
}

void Person::consider_emptying_inventory(vector<AI_Choice>& choices) const{
    if(goal.empty_inventory_can_interrupt()){
        if(!has_inventory_space()){
            choices.push_back(AI_Choice(AI_Goal::Type::EMPTY_INVENTORY,Game_Constants::PRIORITY_EMPTY_INVENTORY));
        }
    }
}

vector<Coords<uint32_t>> Person::consider_eating(vector<AI_Choice>& choices) const{
    vector<Coords<uint32_t>> forage_chunk_coords;

    if(!is_full()){
        if(has_food()){
            if(goal.eat_can_interrupt()){
                choices.push_back(AI_Choice(AI_Goal::Type::EAT,Game_Constants::PRIORITY_EAT));
            }
        }
        else{
            if(goal.forage_can_interrupt()){
                forage_chunk_coords=Chunk::get_zone_chunk_coords(get_chunk_x(),get_chunk_y(),Game_Constants::FORAGE_ZONE_RANGE);

                put_our_chunk_first(forage_chunk_coords);

                vector<Tile::Type> desired_tile_types;
                desired_tile_types.push_back(Tile::Type::WHEAT);
                filter_zone_chunks(forage_chunk_coords,desired_tile_types);
            }

            if(forage_zone_has_food(forage_chunk_coords)){
                choices.push_back(AI_Choice(AI_Goal::Type::FORAGE,Game_Constants::PRIORITY_EAT));
            }
            else{
                if(goal.eat_at_home_can_interrupt()){
                    const Civilization& civilization=Game::get_civilization(get_parent_civilization());

                    if(civilization.has_food()){
                        choices.push_back(AI_Choice(AI_Goal::Type::EAT_AT_HOME,Game_Constants::PRIORITY_EAT));
                    }
                }
            }
        }
    }

    return forage_chunk_coords;
}

uint32_t Person::target_scan(vector<AI_Choice>& choices,RNG& rng,const Quadtree<int32_t,uint32_t>& quadtree,uint32_t index) const{
    if(goal.retreat_can_interrupt() || goal.attack_person_melee_can_interrupt()){
        vector<uint32_t> targets_people;

        Collision_Rect<int32_t> box_sight=get_sight_box();

        quadtree.get_objects(targets_people,box_sight,index);

        vector<AI_Target> valid_targets;

        //Note that scores must never be negative
        int32_t friendly_score=0;
        int32_t enemy_score=0;

        unordered_set<uint32_t> collisions;

        for(uint32_t i=0;i<targets_people.size();i++){
            if(!collisions.count(targets_people[i])){
                collisions.emplace(targets_people[i]);

                const Person& person=Game::get_person(targets_people[i]);

                if(person.is_alive() && Collision::check_rect(box_sight,person.get_box())){
                    int32_t person_score=(int32_t)person.get_health()+(int32_t)person.get_attack()*3+(int32_t)person.get_defense()*2;

                    if(get_parent_civilization()==person.get_parent_civilization()){
                        friendly_score+=person_score;
                    }
                    else{
                        if(could_damage(person)){
                            uint64_t person_distance=Int_Math::distance_between_points_no_sqrt(box.center_x(),box.center_y(),person.get_box().center_x(),person.get_box().center_y());

                            valid_targets.push_back(AI_Target(targets_people[i],person_distance,person.get_health(),person.get_attack(),person.get_defense()));
                        }

                        enemy_score+=person_score;
                    }
                }
            }
        }

        //Score advantage will be one of:
        //1 if there is no advantage or disadvantage
        //Greater than 1 if there is an advantage, with the number representing the ratio of friendly to enemy score
        //Less than -1 if there is a disadvantage, with the (absolute value of the) number representing the ratio of enemy to friendly score
        int32_t score_advantage=1;
        if(enemy_score>0 && friendly_score>enemy_score){
            score_advantage=friendly_score/enemy_score;
        }
        else if(friendly_score>0 && friendly_score<enemy_score){
            score_advantage=-enemy_score/friendly_score;

            //If the score ratio is negative but rounds to -1, just call it 1 (meaning no advantage or disadvantage)
            if(score_advantage==-1){
                score_advantage=1;
            }
        }

        uint32_t target=0;

        if(valid_targets.size()>0){
            Sorting::quick_sort(valid_targets);

            uint32_t valid_target_index=rng.weighted_random_range(0,valid_targets.size()-1,valid_targets.size()-1,RNG::Weight::NORMAL);

            target=valid_targets[valid_target_index].id;

            if(goal.attack_person_melee_can_interrupt()){
                if(score_advantage>=Game_Constants::AI_COMBAT_SCORE_RATIO_OVERWHELMING){
                    choices.push_back(AI_Choice(AI_Goal::Type::ATTACK_PERSON_MELEE,Game_Constants::PRIORITY_ATTACK_PERSON_MELEE_WITH_OVERWHELMING_ADVANTAGE));
                }
                else if(score_advantage>=Game_Constants::AI_COMBAT_SCORE_RATIO_MAJOR){
                    choices.push_back(AI_Choice(AI_Goal::Type::ATTACK_PERSON_MELEE,Game_Constants::PRIORITY_ATTACK_PERSON_MELEE_WITH_MAJOR_ADVANTAGE));
                }
                else if(score_advantage>1){
                    choices.push_back(AI_Choice(AI_Goal::Type::ATTACK_PERSON_MELEE,Game_Constants::PRIORITY_ATTACK_PERSON_MELEE_WITH_MINOR_ADVANTAGE));
                }
                else{
                    choices.push_back(AI_Choice(AI_Goal::Type::ATTACK_PERSON_MELEE,Game_Constants::PRIORITY_ATTACK_PERSON_MELEE_WITH_NO_ADVANTAGE));
                }
            }
        }

        if(goal.retreat_can_interrupt()){
            int32_t priority_retreat=0;

            if(health_low()){
                priority_retreat+=Game_Constants::PRIORITY_RETREAT_WITH_LOW_HEALTH;
            }

            //If we have a score disadvantage
            if(score_advantage<-1){
                int32_t score_ratio=-score_advantage;

                if(score_ratio>=Game_Constants::AI_COMBAT_SCORE_RATIO_OVERWHELMING){
                    priority_retreat+=Game_Constants::PRIORITY_RETREAT_WITH_OVERWHELMING_DISADVANTAGE;
                }
                else if(score_ratio>=Game_Constants::AI_COMBAT_SCORE_RATIO_MAJOR){
                    priority_retreat+=Game_Constants::PRIORITY_RETREAT_WITH_MAJOR_DISADVANTAGE;
                }
                else{
                    priority_retreat+=Game_Constants::PRIORITY_RETREAT_WITH_MINOR_DISADVANTAGE;
                }
            }

            if(priority_retreat>0){
                const City& city=Game::get_city(get_parent_city());

                if(Int_Math::distance_between_points_no_sqrt((int32_t)get_tile_x(),(int32_t)get_tile_y(),
                                                             (int32_t)city.get_tile_x(),(int32_t)city.get_tile_y())>Game_Constants::HOME_DEFENSE_RANGE){
                    choices.push_back(AI_Choice(AI_Goal::Type::RETREAT,priority_retreat));
                }
            }
        }

        return target;
    }
    else{
        return 0;
    }
}

void Person::set_new_goal(RNG& rng,AI_Goal::Type new_goal_type,uint32_t target,vector<Coords<uint32_t>> forage_chunk_coords){
    goal.set_type(new_goal_type);

    if(goal.is_gather()){
        vector<Coords<uint32_t>> gather_chunk_coords;

        const City& city=Game::get_city(parent_city);

        if(city.get_gather_zone_tile_count(goal.get_goal_tile_type())>0){
            gather_chunk_coords=Chunk::get_zone_chunk_coords(city.get_chunk_x(),city.get_chunk_y(),Game_Constants::GATHER_ZONE_RANGE);

            Sorting::shuffle(rng,gather_chunk_coords);

            put_our_chunk_first(gather_chunk_coords);

            vector<Tile::Type> desired_tile_types;
            desired_tile_types.push_back(goal.get_goal_tile_type());
            filter_zone_chunks(gather_chunk_coords,desired_tile_types);
        }

        find_tile(rng,gather_chunk_coords);
    }
    else if(goal.is_empty_inventory()){
        const City& city=Game::get_city(get_parent_city());

        goal.set_coords_pixels(Coords<int32_t>(city.get_center_x(),city.get_center_y()));
    }
    else if(goal.is_eat()){
        goal.set_coords_pixels(Coords<int32_t>(box.center_x(),box.center_y()));
    }
    else if(goal.is_eat_at_home()){
        const City& city=Game::get_city(get_parent_city());

        goal.set_coords_pixels(Coords<int32_t>(city.get_center_x(),city.get_center_y()));
    }
    else if(goal.is_forage()){
        find_tile(rng,forage_chunk_coords);
    }
    else if(goal.is_retreat()){
        const Civilization& civilization=Game::get_civilization(get_parent_civilization());

        vector<uint32_t> civ_cities=civilization.get_cities();

        //If we have multiple cities
        if(civ_cities.size()>1){
            put_our_city_first(civ_cities);

            uint32_t city_index=rng.weighted_random_range(0,civ_cities.size()-1,0,RNG::Weight::NORMAL);

            const City& city=Game::get_city(city_index);

            goal.set_coords_pixels(Coords<int32_t>(city.get_center_x(),city.get_center_y()));
        }
        //If we have only one city
        else{
            if(rng.random_range(0,99)<Game_Constants::RETREAT_HOME_CHANCE){
                const City& city=Game::get_city(get_parent_city());

                goal.set_coords_pixels(Coords<int32_t>(city.get_center_x(),city.get_center_y()));
            }
            else{
                int32_t retreat_zone_x=box.center_x()-(Game_Constants::RETREAT_ZONE_RANGE*(int32_t)Game_Constants::TILE_SIZE);
                int32_t retreat_zone_y=box.center_y()-(Game_Constants::RETREAT_ZONE_RANGE*(int32_t)Game_Constants::TILE_SIZE);
                int32_t retreat_zone_width=Game_Constants::RETREAT_ZONE_RANGE*(int32_t)Game_Constants::TILE_SIZE*2;
                int32_t retreat_zone_height=Game_Constants::RETREAT_ZONE_RANGE*(int32_t)Game_Constants::TILE_SIZE*2;

                if(retreat_zone_x<0){
                    retreat_zone_x=0;
                }
                if(retreat_zone_y<0){
                    retreat_zone_y=0;
                }
                if(retreat_zone_x+retreat_zone_width>=Game::get_world_width()){
                    retreat_zone_width=Game::get_world_width()-1-retreat_zone_x;
                }
                if(retreat_zone_y+retreat_zone_height>=Game::get_world_height()){
                    retreat_zone_height=Game::get_world_height()-1-retreat_zone_y;
                }

                int32_t x=(int32_t)rng.random_range((uint32_t)retreat_zone_x,uint32_t(retreat_zone_x+retreat_zone_width));
                int32_t y=(int32_t)rng.random_range((uint32_t)retreat_zone_y,uint32_t(retreat_zone_y+retreat_zone_height));

                goal.set_coords_pixels(Coords<int32_t>(x,y));
            }
        }
    }
    else if(goal.is_attack_person_melee()){
        goal.set_person_index(target);
    }
}

void Person::ai(RNG& rng,const Quadtree<int32_t,uint32_t>& quadtree,uint32_t frame,uint32_t index){
    if(is_alive()){
        if(allowed_to_select_ai_goal(frame,index)){
            vector<AI_Choice> choices;

            consider_ignoring(choices);

            consider_gathering(choices);

            consider_emptying_inventory(choices);

            vector<Coords<uint32_t>> forage_chunk_coords=consider_eating(choices);

            uint32_t target=target_scan(choices,rng,quadtree,index);

            if(choices.size()>0){
                Sorting::quick_sort(choices);

                uint32_t choice_index=rng.weighted_random_range(0,choices.size()-1,choices.size()-1,RNG::Weight::WEAK);

                if(choices[choice_index].goal_type!=AI_Goal::Type::NONE){
                    set_new_goal(rng,choices[choice_index].goal_type,target,forage_chunk_coords);
                }
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
                //If the goal hasn't counted down any yet
                if(goal.is_first_count()){
                    force+=Int_Vector(get_move_force(),get_angle_to_goal());
                }
                //If the goal has counted down at least once, a goal we reached and started counting down is no longer in range
                else{
                    abandon_goal();
                }
            }
        }
        else{
            brake();
        }
    }
}

AI_Target::AI_Target(uint32_t new_id,uint64_t new_distance,int16_t new_health,int16_t new_attack,int16_t new_defense){
    id=new_id;
    distance=new_distance;
    health=new_health;
    attack=new_attack;
    defense=new_defense;
}

bool AI_Target::operator<=(const AI_Target& target) const{
    uint32_t score=0;

    if(health>=target.health){
        score+=Game_Constants::AI_TARGET_SELECTION_WEIGHT_HEALTH;
    }

    if(attack>=target.attack){
        score+=Game_Constants::AI_TARGET_SELECTION_WEIGHT_ATTACK;
    }

    if(defense>=target.defense){
        score+=Game_Constants::AI_TARGET_SELECTION_WEIGHT_DEFENSE;
    }

    if(distance>=target.distance){
        score+=Game_Constants::AI_TARGET_SELECTION_WEIGHT_DISTANCE;
    }

    if(score>=Game_Constants::AI_TARGET_SELECTION_WEIGHT_NEEDED){
        return true;
    }
    else{
        return false;
    }
}
