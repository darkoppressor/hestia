/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef region_h
#define region_h

#include <coords.h>

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

    Biome biome;

    std::vector<Coords<std::uint32_t>> chunks;

public:

    Region();

    Ground get_ground();
    std::string get_ground_string(Ground ground);

    Biome get_biome();
    void set_biome(Biome new_biome);

    void add_chunk(const Coords<std::uint32_t>& coords);
};

#endif
