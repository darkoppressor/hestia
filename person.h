/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef person_h
#define person_h

#include "int_math.h"
#include "ai.h"
#include "inventory.h"
#include "tile.h"

#include <collision.h>
#include <rng.h>
#include <coords.h>
#include <quadtree.h>

#include <cstdint>
#include <vector>

class Person{
private:

    std::uint32_t parent_city;

    Collision_Rect<std::int32_t> box;

    Int_Vector velocity;
    Int_Vector acceleration;
    Int_Vector force;

    Inventory inventory;

    AI_Goal goal;

    //When a person dies, Game::add_remove_objects will set exists to false
    bool exists;

    std::int16_t health;
    std::uint8_t hunger;

public:

    Person();
    Person(std::uint32_t new_parent,const Collision_Rect<std::int32_t>& new_box);

    std::uint32_t get_parent_city() const;
    void set_parent_city(std::uint32_t new_parent);

    std::uint32_t get_parent_civilization() const;

    Collision_Rect<std::int32_t> get_box() const;
    Collision_Rect<std::int32_t> get_sight_box() const;

    std::uint32_t get_item_count() const;
    std::uint32_t get_item_count(Inventory::Item_Type item_type) const;
    bool has_inventory_space(std::uint32_t amount=1) const;
    bool has_food() const;
    std::uint32_t add_item(Inventory::Item_Type item_type,std::uint32_t amount);
    void remove_item(Inventory::Item_Type item_type,std::uint32_t amount);

    //pixels
    std::int32_t get_center_x() const;
    std::int32_t get_center_y() const;

    //chunks
    std::uint32_t get_chunk_x() const;
    std::uint32_t get_chunk_y() const;

    //tiles
    std::uint32_t get_tile_x() const;
    std::uint32_t get_tile_y() const;

    bool get_exists() const;
    void set_exists(bool new_exists);

    std::int32_t get_max_speed() const;
    std::int32_t get_move_force() const;

    bool is_alive() const;
    bool health_low() const;
    void damage(std::int16_t attack);

    std::int16_t get_health() const;
    std::int16_t get_attack() const;
    std::int16_t get_defense() const;

    bool is_full() const;
    bool is_hungry() const;
    bool is_starving() const;
    void eat();

    void process_biology();

    bool has_goal() const;
    bool is_goal_valid() const;

    //pixels
    //Returns the range^2
    std::uint64_t get_goal_range() const;
    //pixels
    //Returns coordinates of -1,-1 if the goal coordinates could not be determined
    Coords<std::int32_t> get_goal_coords() const;
    bool goal_within_range() const;
    std::int32_t get_angle_to_goal() const;

    void notify_of_person_death(std::uint32_t index);
    void abandon_goal();
    void complete_goal();

    void brake();
    void accelerate();
    void movement();

    void render() const;

    //  AI  //

    void put_our_chunk_first(std::vector<Coords<std::uint32_t>>& chunk_coords) const;
    //Remove the chunk coordinates for any chunks that have none of the desired tile types
    void filter_zone_chunks(std::vector<Coords<std::uint32_t>>& chunk_coords,const std::vector<Tile::Type>& desired_tile_types) const;
    bool forage_zone_has_food(const std::vector<Coords<std::uint32_t>>& chunk_coords) const;
    void find_tile(RNG& rng,const std::vector<Coords<std::uint32_t>>& chunk_coords);

    bool allowed_to_select_ai_goal(std::uint32_t frame,std::uint32_t index) const;

    void consider_ignoring(std::vector<AI_Choice>& choices) const;
    void consider_gathering(std::vector<AI_Choice>& choices) const;
    void consider_emptying_inventory(std::vector<AI_Choice>& choices) const;
    //Returns the forage chunk coordinates, if any were identified and the forage goal was selected
    //Otherwise, returns an empty vector
    std::vector<Coords<std::uint32_t>> consider_eating(std::vector<AI_Choice>& choices) const;
    //Returns the index for the target, if one was identified, or 0 otherwise
    //This is only used if we are setting a new goal of targeting this target,
    //which is only a possible goal if a target was found,
    //so there is no issue with it returning 0 (a possible target index itself) on failure to identify a target
    std::uint32_t target_scan(std::vector<AI_Choice>& choices,RNG& rng,const Quadtree<std::int32_t,std::uint32_t>& quadtree,std::uint32_t index) const;

    void set_new_goal(RNG& rng,AI_Goal::Type new_goal_type,std::uint32_t target,std::vector<Coords<std::uint32_t>> forage_chunk_coords);

    void ai(RNG& rng,const Quadtree<std::int32_t,std::uint32_t>& quadtree,std::uint32_t frame,std::uint32_t index);
};

#endif
