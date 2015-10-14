/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef tile_h
#define tile_h

#include <cstdint>

class Tile{
public:

    enum class Type : std::uint8_t{
        WHEAT,
        TREE,
        BUILDING_UNFINISHED,
        BUILDING_CITY
    };

private:

    //If the tile is of type BUILDING_UNFINISHED, its parent is a Civilization
    //If the tile is of type BUILDING_CITY, its parent is a City
    //Otherwise, the parent variable is not used
    std::uint32_t parent;

    Type type;

    std::int16_t health;

public:

    Tile();
    Tile(Type new_type);
    Tile(std::uint32_t new_parent,Type new_type);

    std::uint32_t get_parent() const;
    void set_parent(std::uint32_t new_parent);

    Type get_type() const;
    void set_type(Type new_type);
    bool is_gatherable() const;
    bool is_food() const;

    bool is_alive() const;
    bool needs_repair() const;
    void repair();
    void capture_heal();
    void damage(std::int16_t attack);
    void die();

    std::int16_t get_health_max() const;
    std::int16_t get_health() const;
    std::int16_t get_defense() const;

    static bool tile_type_is_building(Type type_to_check);
    bool is_building() const;

    //pixels
    static std::int32_t get_tile_type_size(Type type_to_check);
    std::int32_t get_size() const;

    //pixels
    static std::int32_t get_x(std::uint32_t tile_x);
    static std::int32_t get_y(std::uint32_t tile_y);

    //pixels
    static std::int32_t get_center_x(std::uint32_t tile_x,std::int32_t tile_size);
    static std::int32_t get_center_y(std::uint32_t tile_y,std::int32_t tile_size);

    //pixels
    std::int32_t get_center_x(std::uint32_t tile_x) const;
    std::int32_t get_center_y(std::uint32_t tile_y) const;

    //chunks
    static std::uint32_t get_chunk_x(std::uint32_t tile_x);
    static std::uint32_t get_chunk_y(std::uint32_t tile_y);

    void render(std::uint32_t tile_x,std::uint32_t tile_y) const;
};

#endif
