/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef city_h
#define city_h

#include "tile.h"

#include <coords.h>
#include <rng.h>
#include <collision.h>

#include <cstdint>
#include <vector>

class City{
private:

    std::uint32_t parent_civilization;

    Coords<std::uint32_t> tile;

    std::vector<std::uint32_t> people;

    std::uint32_t breeding_counter;

    std::uint32_t gather_zone_wheat;
    std::uint32_t gather_zone_tree;

public:

    City();
    City(std::uint32_t new_parent);

    std::uint32_t get_parent_civilization() const;
    void set_parent_civilization(std::uint32_t new_parent);

    Coords<std::uint32_t> get_tile() const;
    void set_tile(const Coords<std::uint32_t>& new_tile);

    const std::vector<std::uint32_t>& get_people() const;
    void add_person(std::uint32_t person);
    void remove_person(std::uint32_t person);

    //pixels
    std::int32_t get_size() const;

    //pixels
    std::int32_t get_x() const;
    std::int32_t get_y() const;

    //pixels
    std::int32_t get_center_x() const;
    std::int32_t get_center_y() const;

    //chunks
    std::uint32_t get_chunk_x() const;
    std::uint32_t get_chunk_y() const;

    Collision_Rect<std::int32_t> get_spawn_zone() const;

    std::uint32_t get_population() const;

    void breed(std::uint32_t index,RNG& rng);

    std::uint32_t get_gather_zone_tile_count(Tile::Type tile_type) const;
    std::vector<Coords<std::uint32_t>> get_gather_zone_chunk_coords() const;
    bool allowed_to_update_gather_zone(std::uint32_t frame,std::uint32_t index) const;
    void update_gather_zone(std::uint32_t frame,std::uint32_t index);
};

#endif
