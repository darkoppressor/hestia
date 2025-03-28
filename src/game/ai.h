/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef ai_h
#define ai_h

#include "tile.h"

#include <collision/coords.h>

#include <cstdint>
#include <string>

class AI_Goal {
    public:
        enum class Type : std::uint8_t {
            NONE,
            GATHER_WHEAT,
            GATHER_TREE,
            EMPTY_INVENTORY,
            EAT,
            EAT_AT_HOME,
            FORAGE,
            RETREAT,
            ATTACK_PERSON_MELEE,
            ATTACK_BUILDING_MELEE,
            BUILD,
            REPAIR
        };

    private:
        Type type;

        // These represent different kinds of coordinates (or other data) depending on type
        // Tile: the tile coordinates
        // City: the pixel coordinates
        // Person: x represents the person's index, y is unused
        Coords<std::uint32_t> coords;

        std::uint32_t counter;

    public:
        AI_Goal ();

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        std::string get_type_string() const;

        bool is_in_progress() const;

        bool is_gather_wheat() const;
        bool is_gather_tree() const;
        bool target_is_person() const;
        bool is_combat() const;

        bool is_gather() const;
        bool is_empty_inventory() const;
        bool is_eat() const;
        bool is_eat_at_home() const;
        bool is_forage() const;
        bool is_retreat() const;
        bool is_attack_person_melee() const;
        bool is_attack_building_melee() const;
        bool is_build() const;
        bool is_repair() const;

        bool gather_can_interrupt() const;
        bool empty_inventory_can_interrupt() const;
        bool eat_can_interrupt() const;
        bool eat_at_home_can_interrupt() const;
        bool forage_can_interrupt() const;
        bool retreat_can_interrupt() const;
        bool attack_person_melee_can_interrupt() const;
        bool attack_building_melee_can_interrupt() const;
        bool build_can_interrupt() const;
        bool repair_can_interrupt() const;

        Tile::Type get_goal_tile_type() const;

        void reset_counter();
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

        // Returns true if the countdown completed,
        // or false if the countdown did not complete
        bool countdown();
};

class AI_Choice {
    public:
        AI_Goal::Type goal_type;

        std::int32_t priority;

        AI_Choice (AI_Goal::Type new_goal_type, std::int32_t new_priority);

        // Implemented for compatibility with quick_sort
        bool operator<= (const AI_Choice& choice) const;
};

#endif
