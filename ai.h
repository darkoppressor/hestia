/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ai_h
#define ai_h

#include "tile.h"

#include <coords.h>

#include <cstdint>

class AI_Goal{
public:

    enum class Type : std::uint8_t{
        NONE,
        GATHER_WHEAT,
        GATHER_TREE,
        EMPTY_INVENTORY,
        EAT,
        EAT_AT_HOME,
        FORAGE,
        RETREAT,
        ATTACK_PERSON_MELEE
    };

private:

    Type type;

    //These represent different kinds of coordinates depending on type
    //Tile: the tile coordinates
    //City: the pixel coordinates
    //Person: x represents the person's index, y is unused
    Coords<std::uint32_t> coords;

    std::uint32_t counter;

public:

    AI_Goal();

    bool is_in_progress() const;

    bool is_gather_wheat() const;
    bool is_gather_tree() const;
    bool target_is_person() const;

    bool is_gather() const;
    bool is_empty_inventory() const;
    bool is_eat() const;
    bool is_eat_at_home() const;
    bool is_forage() const;
    bool is_retreat() const;
    bool is_attack_person_melee() const;

    bool gather_can_interrupt() const;
    bool empty_inventory_can_interrupt() const;
    bool eat_can_interrupt() const;
    bool eat_at_home_can_interrupt() const;
    bool forage_can_interrupt() const;
    bool retreat_can_interrupt() const;
    bool attack_person_melee_can_interrupt() const;

    bool is_combat() const;

    Tile::Type get_goal_tile_type() const;

    void set_type(Type new_type);

    void clear_goal();

    Coords<std::uint32_t> get_coords_tiles() const;
    Coords<std::int32_t> get_coords_pixels() const;
    std::uint32_t get_person_index() const;

    void set_coords_tiles(const Coords<std::uint32_t>& new_coords);
    void set_coords_pixels(const Coords<std::int32_t>& new_coords);
    void set_person_index(std::uint32_t new_index);

    std::uint32_t get_counter_max() const;

    bool is_first_count() const;

    //Returns true if the countdown completed,
    //or false if the countdown did not complete
    bool countdown();
};

class AI_Choice{
public:

    AI_Goal::Type goal_type;

    std::int32_t priority;

    AI_Choice(AI_Goal::Type new_goal_type,std::int32_t new_priority);

    //Implemented for compatibility with quick_sort
    bool operator<=(const AI_Choice& choice) const;
};

#endif
