/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_constants.h"

#include <game_constants_loader.h>
#include <engine_strings.h>
#include <engine_data.h>

using namespace std;

uint32_t Game_Constants::TILE_SIZE=0;
uint32_t Game_Constants::CHUNK_SIZE=0;
uint32_t Game_Constants::BUILDING_SIZE=0;
uint32_t Game_Constants::PERSON_SIZE=0;

uint32_t Game_Constants::DAY_LENGTH=0;

uint32_t Game_Constants::CITY_POPULATION_MAX=0;
uint32_t Game_Constants::CITY_POPULATION_START=0;
uint32_t Game_Constants::BREEDING_RATE=0;

uint32_t Game_Constants::PERSON_MASS=0;
uint32_t Game_Constants::PERSON_MAX_SPEED=0;
uint32_t Game_Constants::PERSON_MOVE_FORCE=0;

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

    else if(name=="day_length"){
        Game_Constants::DAY_LENGTH=Strings::string_to_unsigned_long(value);
    }

    else if(name=="city_population_max"){
        Game_Constants::CITY_POPULATION_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="city_population_start"){
        Game_Constants::CITY_POPULATION_START=Strings::string_to_unsigned_long(value);
    }
    else if(name=="breeding_rate"){
        Game_Constants::BREEDING_RATE=Strings::string_to_unsigned_long(value);
    }

    else if(name=="person_mass"){
        Game_Constants::PERSON_MASS=Strings::string_to_unsigned_long(value);
    }
    else if(name=="person_max_speed"){
        Game_Constants::PERSON_MAX_SPEED=Strings::string_to_unsigned_long(value);
    }
    else if(name=="person_move_force"){
        Game_Constants::PERSON_MOVE_FORCE=Strings::string_to_unsigned_long(value);
    }
}
