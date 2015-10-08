/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef chunk_h
#define chunk_h

#include "region.h"
#include "tile.h"

#include <string>
#include <cstdint>

class Tile_Counts{
private:

    std::uint32_t wheat;
    std::uint32_t tree;
    std::uint32_t building_unfinished;
    std::uint32_t building_city;

    std::uint32_t& get_tile_reference(Tile::Type tile_type);

public:

    Tile_Counts();

    std::uint32_t get(Tile::Type tile_type) const;
    void increment(Tile::Type tile_type);
    void decrement(Tile::Type tile_type);
};

class Chunk{
private:

    std::uint32_t parent_region;

    Tile_Counts tile_counts;

public:

    Chunk();

    std::uint32_t get_parent_region() const;
    void set_parent_region(std::uint32_t new_parent);

    std::uint32_t get_tile_count(Tile::Type tile_type) const;
    void increment_tile_count(Tile::Type tile_type);
    void decrement_tile_count(Tile::Type tile_type);

    std::string get_ground_string() const;

    //pixels
    std::int32_t get_size() const;

    //pixels
    static std::int32_t get_x(std::uint32_t chunk_x);
    static std::int32_t get_y(std::uint32_t chunk_y);

    void render_ground(std::uint32_t chunk_x,std::uint32_t chunk_y) const;
};

#endif
