/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef tile_h
#define tile_h

#include <color/color.h>

#include <cstdint>
#include <string>
#include <vector>

class Tile {
    public:
        enum class Type : std::uint8_t {
            WHEAT,
            TREE,
            BUILDING_UNFINISHED,
            BUILDING_CITY
        };

    private:
        // If the tile is of type BUILDING_UNFINISHED, its parent is a Civilization
        // If the tile is of type BUILDING_CITY, its parent is a City
        // Otherwise, the parent variable is not used
        std::uint32_t parent;
        Type type;

        std::int16_t health;

    public:
        Tile ();
        Tile (Type new_type);
        Tile (std::uint32_t new_parent, Type new_type);

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        std::uint32_t get_parent() const;
        void set_parent(std::uint32_t new_parent);

        Type get_type() const;
        void set_type(Type new_type);
        bool is_gatherable() const;
        bool is_food() const;

        bool is_alive() const;
        bool health_low() const;
        bool needs_repair() const;
        std::uint32_t repair_count_needed() const;
        void repair();
        void capture_heal();
        void damage(std::int16_t attack);
        void die();

        std::int16_t get_health_max() const;
        std::int16_t get_health() const;
        std::int16_t get_defense() const;

        static bool tile_type_is_building(Type type_to_check);
        bool is_building() const;

        // pixels
        static std::int32_t get_tile_type_size(Type type_to_check);
        std::int32_t get_size() const;

        // pixels
        static std::int32_t get_x(std::uint32_t tile_x);
        static std::int32_t get_y(std::uint32_t tile_y);

        // pixels
        static std::int32_t get_center_x(std::uint32_t tile_x, std::int32_t tile_size);
        static std::int32_t get_center_y(std::uint32_t tile_y, std::int32_t tile_size);

        // pixels
        std::int32_t get_center_x(std::uint32_t tile_x) const;
        std::int32_t get_center_y(std::uint32_t tile_y) const;

        // chunks
        static std::uint32_t get_chunk_x(std::uint32_t tile_x);
        static std::uint32_t get_chunk_y(std::uint32_t tile_y);

        static std::string get_type_string(Type type_to_check);
        static Color* get_type_color(Type type_to_check);

        void write_info_string(std::string& text) const;

        void render(std::uint32_t tile_x, std::uint32_t tile_y, bool selected) const;
};

#endif
