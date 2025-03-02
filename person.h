/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef person_h
#define person_h

#include "ai.h"
#include "inventory.h"
#include "tile.h"

#include <int_vector.h>
#include <collision.h>
#include <rng.h>
#include <coords.h>
#include <quadtree.h>
#include <sprite.h>

#include <cstdint>
#include <vector>
#include <string>

class Target_Scan_Result;

class Person {
    private:
        std::uint32_t parent_city;
        Collision_Rect<std::int32_t> box;
        Int_Vector velocity;
        Int_Vector force;
        Inventory inventory;
        AI_Goal goal;

        // When a person dies, Game::add_remove_objects will set exists to false
        bool exists;

        std::int16_t health;
        std::uint8_t hunger;
        Sprite sprite_body_left;
        Sprite sprite_body_right;
        Sprite sprite_body_up;
        Sprite sprite_body_down;
        Sprite sprite_shirt_left;
        Sprite sprite_shirt_right;
        Sprite sprite_shirt_up;
        Sprite sprite_shirt_down;

    public:
        Person ();
        Person (std::uint32_t new_parent, const Collision_Rect<std::int32_t>& new_box);

        void setup_sprites();

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        std::uint32_t get_parent_city() const;
        void set_parent_city(std::uint32_t new_parent);

        std::uint32_t get_parent_civilization() const;

        Collision_Rect<std::int32_t> get_box() const;
        Collision_Rect<std::int32_t> get_sight_box() const;

        std::uint32_t get_item_count() const;
        std::uint32_t get_item_count(Inventory::Item_Type item_type) const;
        bool has_inventory_space(std::uint32_t amount = 1) const;
        bool is_inventory_space_low() const;
        bool has_item_to_deposit() const;
        bool has_food() const;
        std::uint32_t add_item(Inventory::Item_Type item_type, std::uint32_t amount);
        void remove_item(Inventory::Item_Type item_type, std::uint32_t amount);

        bool has_build_material() const;
        bool has_repair_material() const;

        // pixels
        std::int32_t get_center_x() const;
        std::int32_t get_center_y() const;

        // chunks
        std::uint32_t get_chunk_x() const;
        std::uint32_t get_chunk_y() const;

        // tiles
        std::uint32_t get_tile_x() const;
        std::uint32_t get_tile_y() const;

        bool get_exists() const;
        void set_exists(bool new_exists);

        std::int32_t get_max_speed() const;
        std::int32_t get_move_force() const;

        bool is_alive() const;
        bool health_low() const;
        void damage(std::int16_t attack, bool direct = false);

        std::int16_t get_health_max() const;
        std::int16_t get_health() const;
        std::int16_t get_attack() const;
        std::int16_t get_defense() const;

        bool could_damage_person(const Person& person) const;
        bool could_damage_tile(const Tile& tile) const;

        bool home_was_recently_captured() const;

        bool is_full() const;
        bool is_hungry() const;
        bool is_starving() const;
        void eat();

        void process_biology();

        bool is_friends_with_person(std::uint32_t person_index) const;
        bool is_enemies_with_person(std::uint32_t person_index) const;
        bool is_neutral_towards_person(std::uint32_t person_index) const;

        bool is_friends_with_tile(const Coords<std::uint32_t>& tile_coords) const;
        bool is_enemies_with_tile(const Coords<std::uint32_t>& tile_coords) const;
        bool is_neutral_towards_tile(const Coords<std::uint32_t>& tile_coords) const;

        bool has_goal() const;
        bool is_goal_valid() const;

        // pixels
        // Returns the range^2
        std::uint64_t get_goal_range() const;
        // pixels
        // Returns the distance^2
        std::uint64_t get_goal_distance() const;
        // pixels
        // Returns coordinates of -1,-1 if the goal coordinates could not be determined
        Coords<std::int32_t> get_goal_coords() const;
        bool goal_within_range() const;
        std::int32_t get_angle_to_goal() const;

        bool is_at_home() const;

        void notify_of_person_death(std::uint32_t index);
        // Takes one piece of food from civilization inventory if possible and needed
        // This does not check if we are actually near our city
        void grab_some_food();
        void abandon_goal();
        void complete_goal(RNG& rng);

        void brake();
        void accelerate();
        void movement();

        void write_info_string(std::string& text) const;

        std::string get_facing_direction() const;

        void animate();

        void render(bool selected) const;

        // AI  //

        void put_our_city_first(std::vector<std::uint32_t>& cities) const;
        void put_our_chunk_first(std::vector<Coords<std::uint32_t>>& chunk_coords) const;
        // Remove the chunk coordinates for any chunks that have none of the desired tile types
        void filter_zone_chunks(std::vector<Coords<std::uint32_t>>& chunk_coords,
                                const std::vector<Tile::Type>& desired_tile_types) const;
        bool forage_zone_has_food(const std::vector<Coords<std::uint32_t>>& chunk_coords) const;
        void find_tile(RNG& rng, const std::vector<Coords<std::uint32_t>>& chunk_coords);

        bool allowed_to_select_ai_goal(std::uint32_t frame, std::uint32_t our_index) const;

        void consider_ignoring(std::vector<AI_Choice>& choices) const;
        void consider_gathering(std::vector<AI_Choice>& choices) const;
        void consider_emptying_inventory(std::vector<AI_Choice>& choices) const;
        // Returns the forage chunk coordinates, if any were identified and the forage goal was selected
        // Otherwise, returns an empty vector
        std::vector<Coords<std::uint32_t>> consider_eating(std::vector<AI_Choice>& choices) const;
        // Returns a Target_Scan_Result, which contains:
        // 1. The index for the target person, if one was identified, or 0 otherwise
        // This is only used if we are setting a new goal of targeting this person,
        // which is only a possible goal if a target person was found,
        // so there is no issue with it returning 0 (a possible target person index itself) on failure to identify a
        // target person
        // 2. The tile coordinates of the city building tile to target, if one was identified, or 0,0 otherwise
        // This is only used if we are setting a new goal of targeting this building tile, just like the target person
        // explained above
        Target_Scan_Result target_scan(std::vector<AI_Choice>& choices, RNG& rng,
                                       const Quadtree<std::int32_t, std::uint32_t>& quadtree,
                                       std::uint32_t our_index) const;
        // Returns the tile coordinates of the unfinished building tile to build, if one was identified, or 0,0
        // otherwise
        // This is only used if we are setting a new goal of building this tile,
        // which is only a possible goal if an unfinished building tile was found,
        // so there is no issue with it returning 0,0 (possible tile coordinates themselves) on failure to identify an
        // unfinished building tile
        Coords<std::uint32_t> consider_building(std::vector<AI_Choice>& choices) const;
        void consider_repairing(std::vector<AI_Choice>& choices) const;

        void set_new_goal(RNG& rng, AI_Goal::Type new_goal_type, Target_Scan_Result target_scan_result,
                          std::vector<Coords<std::uint32_t>> forage_chunk_coords,
                          Coords<std::uint32_t> unfinished_building_coords);

        void ai(RNG& rng, const Quadtree<std::int32_t, std::uint32_t>& quadtree, std::uint32_t frame,
                std::uint32_t our_index);
};

class Target_Scan_Result {
    public:
        std::uint32_t person_index;
        Coords<std::uint32_t> building_tile_coords;

        Target_Scan_Result ();
};

class AI_Target {
    private:
        bool person;

    public:
        std::uint32_t person_index;
        Coords<std::uint32_t> building_tile_coords;
        std::uint64_t distance;
        std::int16_t health;
        std::int16_t attack;
        std::int16_t defense;

        AI_Target (std::uint64_t new_distance, std::int16_t new_health, std::int16_t new_attack,
                   std::int16_t new_defense);

        bool is_person() const;
        void set_person_index(std::uint32_t new_person_index);
        void set_building_tile_coords(const Coords<std::uint32_t>& new_building_tile_coords);

        // Implemented for compatibility with quick_sort
        bool operator<= (const AI_Target& target) const;
};

#endif
