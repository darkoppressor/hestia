/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef city_h
#define city_h

#include "tile.h"

#include <collision/coords.h>
#include <rng/rng.h>
#include <collision/collision.h>

#include <cstdint>
#include <vector>
#include <string>

class City {
    private:
        // When a city is abandoned, exists is set to false
        bool exists;

        std::uint32_t parent_civilization;
        Coords<std::uint32_t> tile;

        std::vector<std::uint32_t> people;

        std::uint32_t breeding_counter;
        std::uint32_t captured_counter;

        std::uint32_t gather_zone_wheat;
        std::uint32_t gather_zone_tree;

    public:
        City ();
        City (std::uint32_t new_parent);

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        bool get_exists() const;
        void set_exists(bool new_exists);

        std::uint32_t get_parent_civilization() const;
        void set_parent_civilization(std::uint32_t new_parent);

        Coords<std::uint32_t> get_tile() const;
        void set_tile(const Coords<std::uint32_t>& new_tile);

        const std::vector<std::uint32_t>& get_people() const;

        void add_person(std::uint32_t person);
        void remove_person(std::uint32_t person);

        // pixels
        std::int32_t get_size() const;

        // pixels
        std::int32_t get_x() const;
        std::int32_t get_y() const;

        // pixels
        std::int32_t get_center_x() const;
        std::int32_t get_center_y() const;

        std::uint32_t get_region() const;

        // chunks
        std::uint32_t get_chunk_x() const;
        std::uint32_t get_chunk_y() const;

        // tiles
        std::uint32_t get_tile_x() const;
        std::uint32_t get_tile_y() const;

        Collision_Rect<std::int32_t> get_spawn_zone() const;

        std::uint32_t get_population() const;
        bool has_maximum_population() const;
        std::uint32_t get_excess_population() const;
        bool has_excess_population() const;

        bool needs_repair() const;
        std::uint32_t repair_count_needed() const;

        bool was_recently_captured() const;
        void set_just_captured();

        void breed(std::uint32_t index, RNG& rng);

        void capture_cooldown();

        std::uint32_t get_gather_zone_tile_count(Tile::Type tile_type) const;
        bool allowed_to_update_gather_zone(std::uint32_t frame, std::uint32_t index) const;
        void update_gather_zone(std::uint32_t frame, std::uint32_t index);

        void write_info_string(std::string& text) const;
};

#endif
