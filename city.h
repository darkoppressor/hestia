/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef city_h
#define city_h

#include "tile.h"

#include <coords.h>

#include <cstdint>
#include <vector>

class City{
private:

    std::uint32_t parent_civilization;

    Coords<std::uint32_t> tile;

    std::vector<std::uint32_t> people;

    ///Tile* get_tile_ptr();

public:

    City();
    City(std::uint32_t new_parent);

    std::uint32_t get_parent_civilization();
    void set_parent_civilization(std::uint32_t new_parent);

    Coords<std::uint32_t> get_tile();
    void set_tile(const Coords<std::uint32_t>& new_tile);

    const std::vector<std::uint32_t>& get_people();
    void add_person(std::uint32_t person);
    void remove_person(std::uint32_t person);

    //pixels
    std::int32_t get_size();

    //pixels
    std::int32_t get_x();
    std::int32_t get_y();

    //pixels
    std::int32_t get_center_x();
    std::int32_t get_center_y();
};

#endif
