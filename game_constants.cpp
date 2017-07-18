/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_constants.h"

#include <game_constants_loader.h>
#include <engine_strings.h>
#include <engine_data.h>

using namespace std;

/// BEGIN SCRIPT-GENERATED CONSTANT INITIALIZATIONS
uint32_t Game_Constants::TILE_SIZE=0;
uint32_t Game_Constants::CHUNK_SIZE=0;
uint32_t Game_Constants::BUILDING_SIZE=0;
uint32_t Game_Constants::PERSON_SIZE=0;
int32_t Game_Constants::PERSON_MOVE_ANIMATION_SPEED_MAX=0;
uint32_t Game_Constants::AI_GOAL_SELECTION_PERIOD=0;
uint32_t Game_Constants::CITY_GATHER_ZONE_UPDATE_PERIOD=0;
uint32_t Game_Constants::CIVILIZATION_NEEDS_UPDATE_PERIOD=0;
uint32_t Game_Constants::DAY_LENGTH=0;
uint32_t Game_Constants::CITY_POPULATION_MAX=0;
uint32_t Game_Constants::CITY_POPULATION_DESIRED=0;
uint32_t Game_Constants::CITY_POPULATION_START=0;
uint32_t Game_Constants::BREEDING_RATE=0;
uint32_t Game_Constants::CAPTURE_COOLDOWN=0;
int32_t Game_Constants::PERSON_MASS=0;
int32_t Game_Constants::PERSON_MAX_SPEED=0;
int32_t Game_Constants::PERSON_MAX_SPEED_COMBAT=0;
int32_t Game_Constants::PERSON_MOVE_FORCE=0;
int32_t Game_Constants::PERSON_MOVE_FORCE_COMBAT=0;
uint32_t Game_Constants::INVENTORY_MAX=0;
uint32_t Game_Constants::INVENTORY_ALMOST_FULL_THRESHOLD=0;
int16_t Game_Constants::PERSON_HEALTH_MAX=0;
int16_t Game_Constants::PERSON_HEALTH_CHANGE_RATE=0;
int16_t Game_Constants::PERSON_ATTACK=0;
int16_t Game_Constants::PERSON_DEFENSE=0;
int16_t Game_Constants::BUILDING_HEALTH_MAX=0;
int16_t Game_Constants::BUILDING_REPAIR_AMOUNT=0;
int16_t Game_Constants::BUILDING_DEFENSE=0;
int16_t Game_Constants::BUILDING_CAPTURED_HEALTH_PERCENTAGE=0;
uint32_t Game_Constants::COST_BUILD=0;
uint32_t Game_Constants::COST_REPAIR=0;
uint32_t Game_Constants::SURPLUS_WHEAT=0;
uint32_t Game_Constants::SURPLUS_BASE_WHEAT=0;
uint32_t Game_Constants::SURPLUS_TREE=0;
uint32_t Game_Constants::SURPLUS_BASE_TREE=0;
uint8_t Game_Constants::HUNGER_FULL=0;
uint8_t Game_Constants::HUNGER_HUNGRY=0;
uint8_t Game_Constants::HUNGER_STARVING=0;
uint32_t Game_Constants::GATHER_ZONE_RANGE=0;
uint32_t Game_Constants::FORAGE_ZONE_RANGE=0;
uint64_t Game_Constants::INTERACTION_RANGE=0;
int32_t Game_Constants::SIGHT_RANGE=0;
uint64_t Game_Constants::HOME_DEFENSE_RANGE=0;
uint32_t Game_Constants::CITY_SPACING=0;
uint32_t Game_Constants::MAINTAIN_TARGET_CHANCE=0;
uint32_t Game_Constants::RETREAT_HOME_CHANCE=0;
int32_t Game_Constants::RETREAT_ZONE_RANGE=0;
uint32_t Game_Constants::AI_TARGET_SELECTION_WEIGHT_NEEDED=0;
uint32_t Game_Constants::AI_TARGET_SELECTION_WEIGHT_HEALTH=0;
uint32_t Game_Constants::AI_TARGET_SELECTION_WEIGHT_ATTACK=0;
uint32_t Game_Constants::AI_TARGET_SELECTION_WEIGHT_DEFENSE=0;
uint32_t Game_Constants::AI_TARGET_SELECTION_WEIGHT_DISTANCE=0;
int32_t Game_Constants::AI_COMBAT_SCORE_RATIO_MAJOR=0;
int32_t Game_Constants::AI_COMBAT_SCORE_RATIO_OVERWHELMING=0;
int32_t Game_Constants::PRIORITY_IGNORE=0;
int32_t Game_Constants::PRIORITY_GATHER=0;
int32_t Game_Constants::PRIORITY_GATHER_SURPLUS=0;
int32_t Game_Constants::PRIORITY_EMPTY_INVENTORY_FULL=0;
int32_t Game_Constants::PRIORITY_EMPTY_INVENTORY_ALMOST_FULL=0;
int32_t Game_Constants::PRIORITY_EMPTY_INVENTORY=0;
int32_t Game_Constants::PRIORITY_EAT=0;
int32_t Game_Constants::PRIORITY_RETREAT_WITH_MINOR_DISADVANTAGE=0;
int32_t Game_Constants::PRIORITY_RETREAT_WITH_MAJOR_DISADVANTAGE=0;
int32_t Game_Constants::PRIORITY_RETREAT_WITH_OVERWHELMING_DISADVANTAGE=0;
int32_t Game_Constants::PRIORITY_RETREAT_WITH_LOW_HEALTH=0;
int32_t Game_Constants::PRIORITY_ATTACK_MELEE_WITH_NO_ADVANTAGE=0;
int32_t Game_Constants::PRIORITY_ATTACK_MELEE_WITH_MINOR_ADVANTAGE=0;
int32_t Game_Constants::PRIORITY_ATTACK_MELEE_WITH_MAJOR_ADVANTAGE=0;
int32_t Game_Constants::PRIORITY_ATTACK_MELEE_WITH_OVERWHELMING_ADVANTAGE=0;
int32_t Game_Constants::PRIORITY_BUILD=0;
int32_t Game_Constants::PRIORITY_REPAIR=0;
uint32_t Game_Constants::GATHER_RATE=0;
uint32_t Game_Constants::EMPTY_INVENTORY_RATE=0;
uint32_t Game_Constants::EAT_RATE=0;
uint32_t Game_Constants::EAT_AT_HOME_RATE=0;
uint32_t Game_Constants::FORAGE_RATE=0;
uint32_t Game_Constants::RETREAT_RATE=0;
uint32_t Game_Constants::ATTACK_PERSON_MELEE_RATE=0;
uint32_t Game_Constants::ATTACK_BUILDING_MELEE_RATE=0;
uint32_t Game_Constants::BUILD_RATE=0;
uint32_t Game_Constants::REPAIR_RATE=0;
double Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR=0.0;
double Game_Constants::RENDER_BUILDING_CITY_COLOR=0.0;
double Game_Constants::RENDER_BUILDING_COLOR_BORDER=0.0;
double Game_Constants::RENDER_BAR_HEIGHT=0.0;
double Game_Constants::RENDER_BAR_THICKNESS=0.0;
double Game_Constants::RENDER_SELECTION_SIZE=0.0;
double Game_Constants::RENDER_SELECTION_BORDER=0.0;
double Game_Constants::RENDER_MINIMAP_BORDER=0.0;
double Game_Constants::RENDER_MINIMAP_CAMERA_RECTANGLE=0.0;
/// END SCRIPT-GENERATED CONSTANT INITIALIZATIONS

void Game_Constants_Loader::set_game_constant(string name,string value){
    if(name=="zoom_rate"){
        Engine_Data::ZOOM_RATE=Strings::string_to_double(value);
    }
    else if(name=="zoom_min"){
        Engine_Data::ZOOM_MIN=Strings::string_to_double(value);
    }
    else if(name=="zoom_max"){
        Engine_Data::ZOOM_MAX=Strings::string_to_double(value);
    }

    /// BEGIN SCRIPT-GENERATED CONSTANT SETUP
    else if(name=="tile_size"){
        Game_Constants::TILE_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="chunk_size"){
        Game_Constants::CHUNK_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="building_size"){
        Game_Constants::BUILDING_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="person_size"){
        Game_Constants::PERSON_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="person_move_animation_speed_max"){
        Game_Constants::PERSON_MOVE_ANIMATION_SPEED_MAX=Strings::string_to_long(value);
    }
    else if(name=="ai_goal_selection_period"){
        Game_Constants::AI_GOAL_SELECTION_PERIOD=Strings::string_to_unsigned_long(value);
    }
    else if(name=="city_gather_zone_update_period"){
        Game_Constants::CITY_GATHER_ZONE_UPDATE_PERIOD=Strings::string_to_unsigned_long(value);
    }
    else if(name=="civilization_needs_update_period"){
        Game_Constants::CIVILIZATION_NEEDS_UPDATE_PERIOD=Strings::string_to_unsigned_long(value);
    }
    else if(name=="day_length"){
        Game_Constants::DAY_LENGTH=Strings::string_to_unsigned_long(value);
    }
    else if(name=="city_population_max"){
        Game_Constants::CITY_POPULATION_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="city_population_desired"){
        Game_Constants::CITY_POPULATION_DESIRED=Strings::string_to_unsigned_long(value);
    }
    else if(name=="city_population_start"){
        Game_Constants::CITY_POPULATION_START=Strings::string_to_unsigned_long(value);
    }
    else if(name=="breeding_rate"){
        Game_Constants::BREEDING_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="capture_cooldown"){
        Game_Constants::CAPTURE_COOLDOWN=Strings::string_to_unsigned_long(value);
    }
    else if(name=="person_mass"){
        Game_Constants::PERSON_MASS=Strings::string_to_long(value);
    }
    else if(name=="person_max_speed"){
        Game_Constants::PERSON_MAX_SPEED=Strings::string_to_long(value);
    }
    else if(name=="person_max_speed_combat"){
        Game_Constants::PERSON_MAX_SPEED_COMBAT=Strings::string_to_long(value);
    }
    else if(name=="person_move_force"){
        Game_Constants::PERSON_MOVE_FORCE=Strings::string_to_long(value);
    }
    else if(name=="person_move_force_combat"){
        Game_Constants::PERSON_MOVE_FORCE_COMBAT=Strings::string_to_long(value);
    }
    else if(name=="inventory_max"){
        Game_Constants::INVENTORY_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="inventory_almost_full_threshold"){
        Game_Constants::INVENTORY_ALMOST_FULL_THRESHOLD=Strings::string_to_unsigned_long(value);
    }
    else if(name=="person_health_max"){
        Game_Constants::PERSON_HEALTH_MAX=Strings::string_to_long(value);
    }
    else if(name=="person_health_change_rate"){
        Game_Constants::PERSON_HEALTH_CHANGE_RATE=Strings::string_to_long(value);
    }
    else if(name=="person_attack"){
        Game_Constants::PERSON_ATTACK=Strings::string_to_long(value);
    }
    else if(name=="person_defense"){
        Game_Constants::PERSON_DEFENSE=Strings::string_to_long(value);
    }
    else if(name=="building_health_max"){
        Game_Constants::BUILDING_HEALTH_MAX=Strings::string_to_long(value);
    }
    else if(name=="building_repair_amount"){
        Game_Constants::BUILDING_REPAIR_AMOUNT=Strings::string_to_long(value);
    }
    else if(name=="building_defense"){
        Game_Constants::BUILDING_DEFENSE=Strings::string_to_long(value);
    }
    else if(name=="building_captured_health_percentage"){
        Game_Constants::BUILDING_CAPTURED_HEALTH_PERCENTAGE=Strings::string_to_long(value);
    }
    else if(name=="cost_build"){
        Game_Constants::COST_BUILD=Strings::string_to_unsigned_long(value);
    }
    else if(name=="cost_repair"){
        Game_Constants::COST_REPAIR=Strings::string_to_unsigned_long(value);
    }
    else if(name=="surplus_wheat"){
        Game_Constants::SURPLUS_WHEAT=Strings::string_to_unsigned_long(value);
    }
    else if(name=="surplus_base_wheat"){
        Game_Constants::SURPLUS_BASE_WHEAT=Strings::string_to_unsigned_long(value);
    }
    else if(name=="surplus_tree"){
        Game_Constants::SURPLUS_TREE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="surplus_base_tree"){
        Game_Constants::SURPLUS_BASE_TREE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="hunger_full"){
        Game_Constants::HUNGER_FULL=Strings::string_to_unsigned_long(value);
    }
    else if(name=="hunger_hungry"){
        Game_Constants::HUNGER_HUNGRY=Strings::string_to_unsigned_long(value);
    }
    else if(name=="hunger_starving"){
        Game_Constants::HUNGER_STARVING=Strings::string_to_unsigned_long(value);
    }
    else if(name=="gather_zone_range"){
        Game_Constants::GATHER_ZONE_RANGE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="forage_zone_range"){
        Game_Constants::FORAGE_ZONE_RANGE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="interaction_range"){
        Game_Constants::INTERACTION_RANGE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="sight_range"){
        Game_Constants::SIGHT_RANGE=Strings::string_to_long(value);
    }
    else if(name=="home_defense_range"){
        Game_Constants::HOME_DEFENSE_RANGE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="city_spacing"){
        Game_Constants::CITY_SPACING=Strings::string_to_unsigned_long(value);
    }
    else if(name=="maintain_target_chance"){
        Game_Constants::MAINTAIN_TARGET_CHANCE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="retreat_home_chance"){
        Game_Constants::RETREAT_HOME_CHANCE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="retreat_zone_range"){
        Game_Constants::RETREAT_ZONE_RANGE=Strings::string_to_long(value);
    }
    else if(name=="ai_target_selection_weight_needed"){
        Game_Constants::AI_TARGET_SELECTION_WEIGHT_NEEDED=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ai_target_selection_weight_health"){
        Game_Constants::AI_TARGET_SELECTION_WEIGHT_HEALTH=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ai_target_selection_weight_attack"){
        Game_Constants::AI_TARGET_SELECTION_WEIGHT_ATTACK=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ai_target_selection_weight_defense"){
        Game_Constants::AI_TARGET_SELECTION_WEIGHT_DEFENSE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ai_target_selection_weight_distance"){
        Game_Constants::AI_TARGET_SELECTION_WEIGHT_DISTANCE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ai_combat_score_ratio_major"){
        Game_Constants::AI_COMBAT_SCORE_RATIO_MAJOR=Strings::string_to_long(value);
    }
    else if(name=="ai_combat_score_ratio_overwhelming"){
        Game_Constants::AI_COMBAT_SCORE_RATIO_OVERWHELMING=Strings::string_to_long(value);
    }
    else if(name=="priority_ignore"){
        Game_Constants::PRIORITY_IGNORE=Strings::string_to_long(value);
    }
    else if(name=="priority_gather"){
        Game_Constants::PRIORITY_GATHER=Strings::string_to_long(value);
    }
    else if(name=="priority_gather_surplus"){
        Game_Constants::PRIORITY_GATHER_SURPLUS=Strings::string_to_long(value);
    }
    else if(name=="priority_empty_inventory_full"){
        Game_Constants::PRIORITY_EMPTY_INVENTORY_FULL=Strings::string_to_long(value);
    }
    else if(name=="priority_empty_inventory_almost_full"){
        Game_Constants::PRIORITY_EMPTY_INVENTORY_ALMOST_FULL=Strings::string_to_long(value);
    }
    else if(name=="priority_empty_inventory"){
        Game_Constants::PRIORITY_EMPTY_INVENTORY=Strings::string_to_long(value);
    }
    else if(name=="priority_eat"){
        Game_Constants::PRIORITY_EAT=Strings::string_to_long(value);
    }
    else if(name=="priority_retreat_with_minor_disadvantage"){
        Game_Constants::PRIORITY_RETREAT_WITH_MINOR_DISADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_retreat_with_major_disadvantage"){
        Game_Constants::PRIORITY_RETREAT_WITH_MAJOR_DISADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_retreat_with_overwhelming_disadvantage"){
        Game_Constants::PRIORITY_RETREAT_WITH_OVERWHELMING_DISADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_retreat_with_low_health"){
        Game_Constants::PRIORITY_RETREAT_WITH_LOW_HEALTH=Strings::string_to_long(value);
    }
    else if(name=="priority_attack_melee_with_no_advantage"){
        Game_Constants::PRIORITY_ATTACK_MELEE_WITH_NO_ADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_attack_melee_with_minor_advantage"){
        Game_Constants::PRIORITY_ATTACK_MELEE_WITH_MINOR_ADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_attack_melee_with_major_advantage"){
        Game_Constants::PRIORITY_ATTACK_MELEE_WITH_MAJOR_ADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_attack_melee_with_overwhelming_advantage"){
        Game_Constants::PRIORITY_ATTACK_MELEE_WITH_OVERWHELMING_ADVANTAGE=Strings::string_to_long(value);
    }
    else if(name=="priority_build"){
        Game_Constants::PRIORITY_BUILD=Strings::string_to_long(value);
    }
    else if(name=="priority_repair"){
        Game_Constants::PRIORITY_REPAIR=Strings::string_to_long(value);
    }
    else if(name=="gather_rate"){
        Game_Constants::GATHER_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="empty_inventory_rate"){
        Game_Constants::EMPTY_INVENTORY_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="eat_rate"){
        Game_Constants::EAT_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="eat_at_home_rate"){
        Game_Constants::EAT_AT_HOME_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="forage_rate"){
        Game_Constants::FORAGE_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="retreat_rate"){
        Game_Constants::RETREAT_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="attack_person_melee_rate"){
        Game_Constants::ATTACK_PERSON_MELEE_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="attack_building_melee_rate"){
        Game_Constants::ATTACK_BUILDING_MELEE_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="build_rate"){
        Game_Constants::BUILD_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="repair_rate"){
        Game_Constants::REPAIR_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="render_building_unfinished_color"){
        Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR=Strings::string_to_double(value);
    }
    else if(name=="render_building_city_color"){
        Game_Constants::RENDER_BUILDING_CITY_COLOR=Strings::string_to_double(value);
    }
    else if(name=="render_building_color_border"){
        Game_Constants::RENDER_BUILDING_COLOR_BORDER=Strings::string_to_double(value);
    }
    else if(name=="render_bar_height"){
        Game_Constants::RENDER_BAR_HEIGHT=Strings::string_to_double(value);
    }
    else if(name=="render_bar_thickness"){
        Game_Constants::RENDER_BAR_THICKNESS=Strings::string_to_double(value);
    }
    else if(name=="render_selection_size"){
        Game_Constants::RENDER_SELECTION_SIZE=Strings::string_to_double(value);
    }
    else if(name=="render_selection_border"){
        Game_Constants::RENDER_SELECTION_BORDER=Strings::string_to_double(value);
    }
    else if(name=="render_minimap_border"){
        Game_Constants::RENDER_MINIMAP_BORDER=Strings::string_to_double(value);
    }
    else if(name=="render_minimap_camera_rectangle"){
        Game_Constants::RENDER_MINIMAP_CAMERA_RECTANGLE=Strings::string_to_double(value);
    }
    /// END SCRIPT-GENERATED CONSTANT SETUP
}
