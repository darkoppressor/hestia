/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_h
#define game_h

#include "region.h"
#include "chunk.h"
#include "leader.h"
#include "civilization.h"
#include "city.h"
#include "person.h"
#include "tile.h"
#include "calendar.h"

#include <rng.h>
#include <coords.h>

#include <vector>
#include <string>
#include <cstdint>
#include <map>

class Game{
private:

    struct tile_compare{
        bool operator()(const Coords<std::uint32_t>& a,const Coords<std::uint32_t>& b) const{
            if(a.x!=b.x){
                return a.x<b.x;
            }
            else{
                return a.y<b.y;
            }
        }
    };

public:

    static bool started;

    static std::uint32_t option_rng_seed;

    //chunks
    static std::uint32_t option_world_width;
    static std::uint32_t option_world_height;

    static std::uint32_t option_region_min;
    static std::uint32_t option_region_max;
    static std::uint32_t option_initial_tile_growth;

    static std::uint32_t option_max_leaders;

    static RNG rng;

    static Calendar calendar;

    static std::vector<Region> regions;
    static std::vector<std::vector<Chunk>> chunks;
    static std::vector<Leader> leaders;
    static std::vector<Civilization> civilizations;
    static std::vector<City> cities;
    static std::vector<Person> people;
    static std::map<Coords<std::uint32_t>,Tile,tile_compare> tiles;

    static void clear_world();
    static void setup_leaders();
    static void generate_world();

    //Returns the index for the leader associated with the passed player number,
    //or -1 if the passed player number is invalid
    static std::int32_t get_player_leader(int player_number);

    //pixels
    static std::uint32_t get_world_width();
    static std::uint32_t get_world_height();

    //tiles
    static std::uint32_t get_world_width_tiles();
    static std::uint32_t get_world_height_tiles();

    static bool tile_exists(const Coords<std::uint32_t>& tile_coords);
    static bool tile_coords_are_valid(Tile::Type type,const Coords<std::uint32_t>& tile_coords);

    //Note that this distance is actually the distance^2, as the sqrt operation is skipped
    static uint64_t distance_to_nearest_city(const Coords<std::int32_t>& coords);

    static void tick();
    static void ai();
    static void movement();
    static void events();
    static void animate();
    static void render();

    static void render_to_textures();

    static void update_background();
    static void render_background();

    static bool move_input_state(std::string direction);
};

#endif
