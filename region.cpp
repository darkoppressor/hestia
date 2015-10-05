/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "region.h"

#include <log.h>
#include <engine_strings.h>

using namespace std;

Region::Region(){
    biome=Biome::FREEZING_GRASSLAND;
}

Region::Ground Region::get_ground(){
    if(biome==Biome::FREEZING_GRASSLAND){
        return Ground::DIRT;
    }
    else if(biome==Biome::TROPICAL_GRASSLAND){
        return Ground::GRASS;
    }
    else if(biome==Biome::TEMPERATE_GRASSLAND){
        return Ground::GRASS;
    }
    else if(biome==Biome::FREEZING_FOREST){
        return Ground::GRASS;
    }
    else if(biome==Biome::TROPICAL_FOREST){
        return Ground::GRASS;
    }
    else if(biome==Biome::TEMPERATE_FOREST){
        return Ground::GRASS;
    }
    else if(biome==Biome::GLACIER){
        return Ground::ICE;
    }
    else if(biome==Biome::DESERT){
        return Ground::SAND;
    }
    else if(biome==Biome::ROCKY_WASTELAND){
        return Ground::STONE;
    }
    else{
        Log::add_error("Error accessing ground for biome '"+Strings::num_to_string((uint8_t)biome)+"'");

        return Ground::STONE;
    }
}

string Region::get_ground_string(Ground ground){
    if(ground==Ground::DIRT){
        return "dirt";
    }
    else if(ground==Ground::GRASS){
        return "grass";
    }
    else if(ground==Ground::ICE){
        return "ice";
    }
    else if(ground==Ground::SAND){
        return "sand";
    }
    else if(ground==Ground::STONE){
        return "stone";
    }
    else{
        return "";
    }
}

void Region::set_biome(Biome new_biome){
    biome=new_biome;
}

void Region::add_chunk(const Coords<uint32_t>& coords){
    chunks.push_back(coords);
}
