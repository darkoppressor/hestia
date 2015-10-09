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

    static std::uint32_t AI_TARGET_SCAN_PERIOD;
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

    static std::int16_t PERSON_HEALTH_MAX;
    static std::int16_t PERSON_HEALTH_CHANGE_RATE;
    static std::int16_t PERSON_ATTACK;
    static std::int16_t PERSON_DEFENSE;

    static std::uint8_t HUNGER_FULL;
    static std::uint8_t HUNGER_HUNGRY;
    static std::uint8_t HUNGER_STARVING;

    static std::uint32_t GATHER_ZONE_RANGE;
    static std::uint32_t FORAGE_ZONE_RANGE;

    static std::uint32_t INTERACTION_RANGE;
    static std::int32_t SIGHT_RANGE;
    static std::uint64_t HOME_DEFENSE_RANGE;

    static std::int32_t ATTACK_PERSON_MELEE_PRIORITY;
    static std::int32_t ATTACK_PERSON_MELEE_WITH_ADVANTAGE_PRIORITY;
    static std::int32_t ATTACK_PERSON_MELEE_WITHOUT_ADVANTAGE_PRIORITY;
    static std::int32_t RETREAT_WITH_DISADVANTAGE_PRIORITY;
    static std::int32_t RETREAT_WITH_LOW_HEALTH_PRIORITY;
    static std::int32_t IGNORE_PRIORITY;

    static std::uint32_t GATHER_RATE;
    static std::uint32_t EMPTY_INVENTORY_RATE;
    static std::uint32_t EAT_RATE;
    static std::uint32_t EAT_AT_HOME_RATE;
    static std::uint32_t FORAGE_RATE;
    static std::uint32_t RETREAT_RATE;
    static std::uint32_t ATTACK_PERSON_MELEE_RATE;
};

#endif
