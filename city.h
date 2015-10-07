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

    ///Tile* get_tile_ptr();

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

    Collision_Rect<int32_t> get_spawn_zone() const;

    std::uint32_t get_population() const;

    void breed(std::uint32_t index,RNG& rng);
};

#endif
