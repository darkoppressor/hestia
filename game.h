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

#include <rng.h>
#include <coords.h>

#include <vector>
#include <string>
#include <cstdint>
#include <map>

class Game{
public:

    static bool started;

    static std::uint32_t option_rng_seed;

    //chunks
    static std::uint32_t option_world_width;
    static std::uint32_t option_world_height;

    static std::uint32_t option_region_min;
    static std::uint32_t option_region_max;

    static std::uint32_t option_max_leaders;

    static RNG rng;

    static std::vector<Region> regions;
    static std::vector<std::vector<Chunk>> chunks;
    static std::vector<Leader> leaders;
    static std::vector<Civilization> civilizations;
    static std::vector<City> cities;
    static std::vector<Person> people;
    static std::map<Coords<std::uint32_t>,Tile> tiles;

    //pixels
    static std::uint32_t get_world_width();
    static std::uint32_t get_world_height();

    static void clear_world();
    static void setup_leaders();
    static void generate_world();

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
