/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef civilization_h
#define civilization_h

#include "inventory.h"

#include <coords.h>

#include <cstdint>
#include <vector>
#include <string>

class Civilization{
private:

    std::uint32_t parent_leader;

    std::vector<std::uint32_t> cities;

    std::vector<Coords<std::uint32_t>> unfinished_buildings;
    //These are all either 0 or 1, but I don't want to use a vector<bool>
    std::vector<std::uint8_t> unfinished_building_flags;

    Inventory inventory;

    //The amount of each item that is needed
    std::uint32_t need_wheat;
    std::uint32_t need_tree;

    //The amount of each item that is desired for surplus (OVER the needed amount)
    std::uint32_t surplus_wheat;
    std::uint32_t surplus_tree;

public:

    Civilization();
    Civilization(std::uint32_t new_parent);

    void add_checksum_data(std::vector<std::uint32_t>& data) const;

    std::uint32_t get_parent_leader() const;
    void set_parent_leader(std::uint32_t new_parent);

    bool is_defeated() const;

    std::vector<std::uint32_t> get_cities() const;
    void add_city(std::uint32_t city);
    void remove_city(std::uint32_t city);

    std::vector<Coords<std::uint32_t>> get_unfinished_buildings() const;
    void add_unfinished_building(Coords<std::uint32_t> tile_coords);
    void remove_unfinished_building(Coords<std::uint32_t> tile_coords);
    bool get_unfinished_building_flag(Coords<std::uint32_t> tile_coords) const;
    void set_unfinished_building_flag(Coords<std::uint32_t> tile_coords,bool new_flag);

    std::uint32_t get_item_count() const;
    std::uint32_t get_item_count(Inventory::Item_Type item_type) const;
    bool has_food() const;
    std::uint32_t add_item(Inventory::Item_Type item_type,std::uint32_t amount);
    void remove_item(Inventory::Item_Type item_type,std::uint32_t amount);

    std::uint32_t get_population() const;

    std::string get_color() const;

    bool is_friends_with(std::uint32_t civilization_index) const;
    bool is_enemies_with(std::uint32_t civilization_index) const;
    bool is_neutral_towards(std::uint32_t civilization_index) const;

    std::uint32_t get_item_need(Inventory::Item_Type item_type) const;
    std::uint32_t get_item_desire(Inventory::Item_Type item_type) const;
    bool is_item_needed(Inventory::Item_Type item_type) const;
    bool is_item_desired(Inventory::Item_Type item_type) const;
    bool allowed_to_update_needs(std::uint32_t frame,std::uint32_t index) const;
    void update_needs(std::uint32_t frame,std::uint32_t index);

    void write_info_string(std::string& text) const;
};

#endif
