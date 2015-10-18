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
#include "inventory.h"
#include "game_order.h"
#include "game_selection.h"
#include "minimap.h"

#include <rng.h>
#include <coords.h>
#include <color.h>
#include <quadtree.h>
#include <image_data.h>

#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <queue>

class Game_City_Distance{
public:

    std::uint32_t index;

    std::uint64_t distance;

    Game_City_Distance(std::uint32_t new_index,std::uint64_t new_distance);

    bool operator<=(const Game_City_Distance& game_city_distance) const;
};

class Game{
public:

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

private:

    static Game_Order::Type order;

    static std::vector<Game_Order> server_game_orders;
    static std::vector<Game_Order> client_game_orders;

    static Game_Selection selection;

    static std::vector<Region> regions;
    static std::vector<std::vector<Chunk>> chunks;
    static std::vector<Leader> leaders;
    static std::vector<Civilization> civilizations;
    static std::vector<City> cities;
    static std::vector<Person> people;
    static std::map<Coords<std::uint32_t>,Tile,tile_compare> tiles;

    static std::queue<std::uint32_t> dead_cities;
    static std::queue<std::uint32_t> dead_people;

    static Quadtree<std::int32_t,std::uint32_t> quadtree;

    static Minimap minimap;

    static void add_remove_objects();

    static void generate_minimap();
    static void update_minimap();
    static void clear_minimap();

public:

    static bool started;

    //Keeps track of the current logic update frame
    //Valid values: 0 to UPDATE_RATE-1
    static std::uint32_t frame;

    static std::string edge_scroll_state;
    static double edge_scroll_speed;
    static bool camera_drag_main;
    static bool camera_drag_minimap;

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

    static std::vector<City> new_cities;
    static std::vector<Person> new_people;
    static std::map<Coords<std::uint32_t>,Tile,tile_compare> new_tiles;

    static void clear_world();
    static void setup_leaders();
    static void generate_world();

    static std::uint32_t get_people_count();
    static std::uint32_t get_city_count();

    static const std::map<Coords<std::uint32_t>,Tile,tile_compare>& get_tiles();

    static const Region& get_region(std::uint32_t index);
    static const Chunk& get_chunk(const Coords<std::uint32_t>& coords);
    static const Leader& get_leader(std::uint32_t index);
    static const Civilization& get_civilization(std::uint32_t index);
    static const City& get_city(std::uint32_t index);
    static const Person& get_person(std::uint32_t index);
    static const Tile& get_tile(const Coords<std::uint32_t>& coords);

    static std::uint32_t get_leader_count();
    static void add_leader(bool player_controlled,std::uint32_t parent_player,const Color& color,const std::vector<Leader::Diplomacy_State>& diplomacy_states);

    //Returns the index for the leader associated with the passed player number,
    //or -1 if the passed player number is invalid
    static std::int32_t get_player_leader(int player_number);

    //Returns the index for our leader,
    //or -1 if our leader could not be determined
    static std::int32_t get_our_leader();

    //pixels
    static std::int32_t get_world_width();
    static std::int32_t get_world_height();

    //tiles
    static std::uint32_t get_world_width_tiles();
    static std::uint32_t get_world_height_tiles();

    static bool tile_exists(const Coords<std::uint32_t>& tile_coords);
    static bool tile_coords_are_valid(Tile::Type type,const Coords<std::uint32_t>& tile_coords);
    static void kill_tile(const Coords<std::uint32_t>& tile_coords);

    static std::uint32_t add_civilization_item(std::uint32_t index,Inventory::Item_Type item_type,std::uint32_t amount);
    static void remove_civilization_item(std::uint32_t index,Inventory::Item_Type item_type,std::uint32_t amount);
    static void set_civilization_unfinished_building_flag(std::uint32_t index,Coords<std::uint32_t> tile_coords,bool new_flag);

    static void damage_person(std::uint32_t index,std::int16_t attack);
    static void damage_tile(const Coords<std::uint32_t>& tile_coords,std::int16_t attack);

    static void repair_tile(const Coords<std::uint32_t>& tile_coords);

    static void handle_city_capture(const Coords<std::uint32_t>& tile_coords,std::uint32_t capturing_civilization_index);
    static void repopulate_city(std::uint32_t city_index);
    static void abandon_city(std::uint32_t city_index);

    //pixels^2
    static uint64_t distance_to_nearest_city(const Coords<std::int32_t>& coords);

    static Coords<std::int32_t> get_mouse_coords_pixels();
    static Coords<std::uint32_t> get_mouse_coords_tiles();

    static Game_Selection get_selection();
    static void set_selection(Game_Selection::Type type,std::uint32_t index);
    static void toggle_selection_follow();
    static void clear_selection();
    static void close_selection_windows();

    static Game_Order::Type get_order();
    static bool has_order();
    static void set_order(Game_Order::Type new_order);
    static void clear_order();
    static void issue_order(const Game_Order& new_order);

    static std::vector<Game_Order> get_server_game_orders();
    static void add_server_game_order(const Game_Order& new_order);
    static void clear_server_game_orders();

    static std::vector<Game_Order> get_client_game_orders();
    static void add_client_game_order(const Game_Order& new_order);
    static void clear_client_game_orders();
    static void execute_client_game_orders();

    static bool is_mouse_over_minimap();
    static void center_camera_on_minimap_position();

    static void tick();
    static void ai();
    static void movement();
    static void events();
    static void animate();
    static void render();

    static void render_to_textures();

    static void update_background();
    static void render_background();

    static std::uint32_t get_checksum();

    static bool move_input_state(std::string direction);
    static bool edge_scroll_input_state(std::string direction);
};

#endif
