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
    static std::uint32_t CIVILIZATION_NEEDS_UPDATE_PERIOD;

    static std::uint32_t DAY_LENGTH;

    static std::uint32_t CITY_POPULATION_MAX;
    static std::uint32_t CITY_POPULATION_DESIRED;
    static std::uint32_t CITY_POPULATION_START;
    static std::uint32_t BREEDING_RATE;
    static std::uint32_t CAPTURE_COOLDOWN;

    static std::int32_t PERSON_MASS;
    static std::int32_t PERSON_MAX_SPEED;
    static std::int32_t PERSON_MAX_SPEED_COMBAT;
    static std::int32_t PERSON_MOVE_FORCE;
    static std::int32_t PERSON_MOVE_FORCE_COMBAT;

    static std::uint32_t INVENTORY_MAX;
    static std::uint32_t INVENTORY_ALMOST_FULL_THRESHOLD;

    static std::int16_t PERSON_HEALTH_MAX;
    static std::int16_t PERSON_HEALTH_CHANGE_RATE;
    static std::int16_t PERSON_ATTACK;
    static std::int16_t PERSON_DEFENSE;

    static std::int16_t BUILDING_HEALTH_MAX;
    static std::int16_t BUILDING_REPAIR_AMOUNT;
    static std::int16_t BUILDING_DEFENSE;

    static std::int16_t BUILDING_CAPTURED_HEALTH_PERCENTAGE;

    static std::uint32_t COST_BUILD;
    static std::uint32_t COST_REPAIR;

    static std::uint32_t SURPLUS_WHEAT;
    static std::uint32_t SURPLUS_TREE;

    static std::uint8_t HUNGER_FULL;
    static std::uint8_t HUNGER_HUNGRY;
    static std::uint8_t HUNGER_STARVING;

    static std::uint32_t GATHER_ZONE_RANGE;
    static std::uint32_t FORAGE_ZONE_RANGE;

    static std::uint64_t INTERACTION_RANGE;
    static std::int32_t SIGHT_RANGE;
    static std::uint64_t HOME_DEFENSE_RANGE;
    static std::uint32_t CITY_SPACING;

    static std::uint32_t MAINTAIN_TARGET_CHANCE;

    static std::uint32_t RETREAT_HOME_CHANCE;
    static std::int32_t RETREAT_ZONE_RANGE;

    static std::uint32_t AI_TARGET_SELECTION_WEIGHT_NEEDED;
    static std::uint32_t AI_TARGET_SELECTION_WEIGHT_HEALTH;
    static std::uint32_t AI_TARGET_SELECTION_WEIGHT_ATTACK;
    static std::uint32_t AI_TARGET_SELECTION_WEIGHT_DEFENSE;
    static std::uint32_t AI_TARGET_SELECTION_WEIGHT_DISTANCE;

    static std::int32_t AI_COMBAT_SCORE_RATIO_MAJOR;
    static std::int32_t AI_COMBAT_SCORE_RATIO_OVERWHELMING;

    static std::int32_t PRIORITY_IGNORE;
    static std::int32_t PRIORITY_GATHER;
    static std::int32_t PRIORITY_GATHER_SURPLUS;
    static std::int32_t PRIORITY_EMPTY_INVENTORY_FULL;
    static std::int32_t PRIORITY_EMPTY_INVENTORY_ALMOST_FULL;
    static std::int32_t PRIORITY_EMPTY_INVENTORY;
    static std::int32_t PRIORITY_EAT;
    static std::int32_t PRIORITY_RETREAT_WITH_MINOR_DISADVANTAGE;
    static std::int32_t PRIORITY_RETREAT_WITH_MAJOR_DISADVANTAGE;
    static std::int32_t PRIORITY_RETREAT_WITH_OVERWHELMING_DISADVANTAGE;
    static std::int32_t PRIORITY_RETREAT_WITH_LOW_HEALTH;
    static std::int32_t PRIORITY_ATTACK_MELEE_WITH_NO_ADVANTAGE;
    static std::int32_t PRIORITY_ATTACK_MELEE_WITH_MINOR_ADVANTAGE;
    static std::int32_t PRIORITY_ATTACK_MELEE_WITH_MAJOR_ADVANTAGE;
    static std::int32_t PRIORITY_ATTACK_MELEE_WITH_OVERWHELMING_ADVANTAGE;
    static std::int32_t PRIORITY_BUILD;
    static std::int32_t PRIORITY_REPAIR;

    static std::uint32_t GATHER_RATE;
    static std::uint32_t EMPTY_INVENTORY_RATE;
    static std::uint32_t EAT_RATE;
    static std::uint32_t EAT_AT_HOME_RATE;
    static std::uint32_t FORAGE_RATE;
    static std::uint32_t RETREAT_RATE;
    static std::uint32_t ATTACK_PERSON_MELEE_RATE;
    static std::uint32_t ATTACK_BUILDING_MELEE_RATE;
    static std::uint32_t BUILD_RATE;
    static std::uint32_t REPAIR_RATE;

    static double RENDER_BUILDING_UNFINISHED_COLOR;
    static double RENDER_BUILDING_CITY_COLOR;
    static double RENDER_BUILDING_COLOR_BORDER;
    static double RENDER_BAR_HEIGHT;
    static double RENDER_BAR_THICKNESS;
    static double RENDER_SELECTION_SIZE;
    static double RENDER_SELECTION_BORDER;
    static double RENDER_MINIMAP_BORDER;
    static double RENDER_MINIMAP_CAMERA_RECTANGLE;
};

#endif
