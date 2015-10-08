/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_constants_h
#define game_constants_h

#include <cstdint>

class Game_Constants{
public:

    static std::uint32_t TILE_SIZE;
    static std::uint32_t CHUNK_SIZE;
    static std::uint32_t BUILDING_SIZE;
    static std::uint32_t PERSON_SIZE;

    static std::uint32_t AI_GOAL_SELECTION_PERIOD;
    static std::uint32_t CITY_GATHER_ZONE_UPDATE_PERIOD;

    static std::uint32_t DAY_LENGTH;

    static std::uint32_t CITY_POPULATION_MAX;
    static std::uint32_t CITY_POPULATION_START;
    static std::uint32_t BREEDING_RATE;

    static std::int32_t PERSON_MASS;
    static std::int32_t PERSON_MAX_SPEED;
    static std::int32_t PERSON_MOVE_FORCE;

    static std::uint32_t INVENTORY_MAX;

    static std::uint32_t GATHER_ZONE_RANGE;

    static std::uint32_t INTERACTION_RANGE;

    static std::uint32_t GATHER_RATE;
    static std::uint32_t EMPTY_INVENTORY_RATE;
};

#endif
