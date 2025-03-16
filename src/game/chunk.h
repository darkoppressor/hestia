/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef chunk_h
#define chunk_h

#include "region.h"
#include "tile.h"

#include <collision/coords.h>

#include <string>
#include <cstdint>
#include <vector>

class Tile_Counts {
    private:
        std::uint32_t wheat;
        std::uint32_t tree;
        std::uint32_t building_unfinished;
        std::uint32_t building_city;

        std::uint32_t& get_tile_reference(Tile::Type tile_type);

    public:
        Tile_Counts ();

        std::uint32_t get(Tile::Type tile_type) const;
        void increment(Tile::Type tile_type);
        void decrement(Tile::Type tile_type);
};

class Chunk {
    private:
        std::uint32_t parent_region;
        Tile_Counts tile_counts;

    public:
        Chunk ();

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        std::uint32_t get_parent_region() const;
        void set_parent_region(std::uint32_t new_parent);

        std::uint32_t get_tile_count(Tile::Type tile_type) const;
        void increment_tile_count(Tile::Type tile_type);
        void decrement_tile_count(Tile::Type tile_type);

        bool has_food() const;

        std::string get_ground_string() const;
        Color* get_ground_color() const;

        // pixels
        std::int32_t get_size() const;

        // pixels
        static std::int32_t get_x(std::uint32_t chunk_x);
        static std::int32_t get_y(std::uint32_t chunk_y);

        // tile coords
        static Coords<std::uint32_t> get_central_building_coords(const Coords<std::uint32_t>& chunk_coords);

        static std::vector<Coords<std::uint32_t>> get_zone_chunk_coords(std::uint32_t chunk_x, std::uint32_t chunk_y,
                                                                        std::uint32_t zone_range);

        void render_ground(std::uint32_t chunk_x, std::uint32_t chunk_y) const;
        void render_border_overlay(std::uint32_t chunk_x, std::uint32_t chunk_y, const std::string& color) const;
};

#endif
