/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef region_h
#define region_h

#include <coords.h>
#include <rng.h>
#include <color.h>

#include <vector>
#include <string>
#include <cstdint>

class Region{
public:

    enum class Biome : std::uint8_t{
        FREEZING_GRASSLAND,
        TROPICAL_GRASSLAND,
        TEMPERATE_GRASSLAND,
        FREEZING_FOREST,
        TROPICAL_FOREST,
        TEMPERATE_FOREST,
        GLACIER,
        DESERT,
        ROCKY_WASTELAND
    };

    enum class Ground : std::uint8_t{
        DIRT,
        GRASS,
        ICE,
        SAND,
        STONE
    };

private:

    std::vector<Coords<std::uint32_t>> chunks;

    Biome biome;

public:

    Region();

    void add_chunk(const Coords<std::uint32_t>& coords);

    Biome get_biome() const;
    void set_biome(Biome new_biome);

    Ground get_ground() const;
    std::string get_ground_string() const;
    Color* get_ground_color() const;

    uint32_t get_tile_growth_chance() const;
    uint32_t get_tile_growth_tree_chance() const;
    void tile_growth(RNG& rng);
};

#endif
