/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "region.h"
#include "game_constants.h"
#include "game.h"
#include "tile.h"

#include <log.h>
#include <engine_strings.h>
#include <object_manager.h>

using namespace std;

Region::Region(){
    has_parent = false;
    parent_civilization = 0;

    biome=Biome::FREEZING_GRASSLAND;
}

void Region::add_checksum_data(vector<uint32_t>& data) const{
    data.push_back((uint32_t)has_parent);
    data.push_back(parent_civilization);

    for(size_t i=0;i<chunks.size();i++){
        data.push_back(chunks[i].x);
        data.push_back(chunks[i].y);
    }

    data.push_back((uint32_t)biome);
}

bool Region::has_parent_civilization() const{
    return has_parent;
}

uint32_t Region::get_parent_civilization() const{
    return parent_civilization;
}

void Region::set_parent_civilization(uint32_t parent){
    has_parent=true;
    parent_civilization=parent;
}

void Region::clear_parent_civilization(){
    has_parent=false;
    parent_civilization=0;
}

void Region::add_chunk(const Coords<uint32_t>& coords){
    chunks.push_back(coords);
}

const vector<Coords<uint32_t>>& Region::get_chunks() const{
    return chunks;
}

Region::Biome Region::get_biome() const{
    return biome;
}

void Region::set_biome(Biome new_biome){
    biome=new_biome;
}

Region::Ground Region::get_ground() const{
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

string Region::get_ground_string() const{
    Ground ground=get_ground();

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

Color* Region::get_ground_color() const{
    string ground_string=get_ground_string();

    if(ground_string.length()>0){
        return Object_Manager::get_color("ground_"+ground_string);
    }
    else{
        return Object_Manager::get_color("ground_grass");
    }
}

uint32_t Region::get_tile_count(Tile::Type tile_type) const{
    uint32_t tile_count = 0;

    for(size_t i=0;i<chunks.size();i++){
        tile_count+=Game::get_chunk(chunks[i]).get_tile_count(tile_type);
    }

    return tile_count;
}

uint32_t Region::get_tile_growth_chance() const{
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

uint32_t Region::get_tile_growth_tree_chance() const{
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

        Tile::Type tile_type=Tile::Type::WHEAT;

        if(rng.random_range(0,99)<get_tile_growth_tree_chance()){
            tile_type=Tile::Type::TREE;
        }

        if(Game::tile_coords_are_valid(tile_type,tile_coords)){
            Game::new_tiles.emplace(std::piecewise_construct,
                                std::forward_as_tuple(tile_coords),std::forward_as_tuple(tile_type));
        }
    }
}

void Region::render_chunk_border_overlays(const string& color_override) const{
    if (has_parent || color_override.length() > 0) {
        string color = color_override;

        if (has_parent && color.length() == 0) {
            color = Game::get_civilization(parent_civilization).get_color();
        }

        for (const auto& it : chunks) {
            Game::get_chunk(it).render_border_overlay(it.x,it.y,color);
        }
    }
}
