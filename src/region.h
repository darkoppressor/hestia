/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef region_h
#define region_h

#include "tile.h"

#include <coords.h>
#include <rng.h>
#include <color.h>

#include <vector>
#include <string>
#include <cstdint>

class Region {
    public:
        enum class Biome : std::uint8_t {
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

        enum class Ground : std::uint8_t {
            DIRT,
            GRASS,
            ICE,
            SAND,
            STONE
        };

    private:
        bool has_parent;
        std::uint32_t parent_civilization;

        std::vector<Coords<std::uint32_t>> chunks;
        Biome biome;

    public:
        Region ();

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        bool has_parent_civilization() const;
        std::uint32_t get_parent_civilization() const;
        void set_parent_civilization(std::uint32_t parent);
        void clear_parent_civilization();

        void add_chunk(const Coords<std::uint32_t>& coords);

        const std::vector<Coords<std::uint32_t>>& get_chunks() const;

        Biome get_biome() const;
        void set_biome(Biome new_biome);

        Ground get_ground() const;
        std::string get_ground_string() const;
        Color* get_ground_color() const;

        std::uint32_t get_tile_count(Tile::Type tile_type) const;

        uint32_t get_tile_growth_chance() const;
        uint32_t get_tile_growth_tree_chance() const;
        void tile_growth(RNG& rng);

        void render_chunk_border_overlays(const std::string& color_override) const;
};

#endif
