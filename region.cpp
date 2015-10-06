/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "region.h"
#include "game_constants.h"
#include "game.h"
#include "tile.h"

#include <log.h>
#include <engine_strings.h>

using namespace std;

Region::Region(){
    biome=Biome::FREEZING_GRASSLAND;
}

void Region::add_chunk(const Coords<uint32_t>& coords){
    chunks.push_back(coords);
}

Region::Biome Region::get_biome(){
    return biome;
}

void Region::set_biome(Biome new_biome){
    biome=new_biome;
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

uint32_t Region::get_tile_growth_chance(){
    if(biome==Biome::FREEZING_GRASSLAND){
        return 5;
    }
    else if(biome==Biome::TROPICAL_GRASSLAND){
        return 20;
    }
    else if(biome==Biome::TEMPERATE_GRASSLAND){
        return 15;
    }
    else if(biome==Biome::FREEZING_FOREST){
        return 10;
    }
    else if(biome==Biome::TROPICAL_FOREST){
        return 20;
    }
    else if(biome==Biome::TEMPERATE_FOREST){
        return 15;
    }
    else if(biome==Biome::GLACIER){
        return 0;
    }
    else if(biome==Biome::DESERT){
        return 0;
    }
    else if(biome==Biome::ROCKY_WASTELAND){
        return 1;
    }
    else{
        Log::add_error("Error accessing tile growth chance for biome '"+Strings::num_to_string((uint8_t)biome)+"'");

        return 1;
    }
}

uint32_t Region::get_tile_growth_tree_chance(){
    if(biome==Biome::FREEZING_GRASSLAND){
        return 20;
    }
    else if(biome==Biome::TROPICAL_GRASSLAND){
        return 15;
    }
    else if(biome==Biome::TEMPERATE_GRASSLAND){
        return 10;
    }
    else if(biome==Biome::FREEZING_FOREST){
        return 90;
    }
    else if(biome==Biome::TROPICAL_FOREST){
        return 90;
    }
    else if(biome==Biome::TEMPERATE_FOREST){
        return 85;
    }
    else if(biome==Biome::GLACIER){
        return 50;
    }
    else if(biome==Biome::DESERT){
        return 50;
    }
    else if(biome==Biome::ROCKY_WASTELAND){
        return 50;
    }
    else{
        Log::add_error("Error accessing tile growth tree chance for biome '"+Strings::num_to_string((uint8_t)biome)+"'");

        return 50;
    }
}

void Region::tile_growth(RNG& rng){
    if(rng.random_range(0,99)<get_tile_growth_chance()){
        //Select a chunk from this region at random
        uint32_t chunk_index=rng.random_range(0,chunks.size()-1);
        Coords<uint32_t> chunk_coords=chunks[chunk_index];

        //Select a tile from the chunk at random
        uint32_t tile_left=chunk_coords.x*Game_Constants::CHUNK_SIZE;
        uint32_t tile_top=chunk_coords.y*Game_Constants::CHUNK_SIZE;
        uint32_t tile_right=tile_left+Game_Constants::CHUNK_SIZE-1;
        uint32_t tile_bottom=tile_top+Game_Constants::CHUNK_SIZE-1;
        Coords<uint32_t> tile_coords(rng.random_range(tile_left,tile_right),rng.random_range(tile_top,tile_bottom));

        if(!Game::tile_exists(tile_coords)){
            Tile::Type tile_type=Tile::Type::WHEAT;

            if(rng.random_range(0,99)<get_tile_growth_tree_chance()){
                tile_type=Tile::Type::TREE;
            }

            Game::tiles.emplace(std::piecewise_construct,
                                std::forward_as_tuple(tile_coords),std::forward_as_tuple(tile_type));
        }
    }
}
