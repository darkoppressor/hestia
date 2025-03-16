/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "city.h"
#include "game_constants.h"
#include "game.h"

#include <engine.h>
#include <strings/engine_strings.h>

using namespace std;

City::City () {
    exists = true;

    parent_civilization = 0;

    breeding_counter = Game_Constants::BREEDING_RATE;
    captured_counter = 0;

    gather_zone_wheat = 0;
    gather_zone_tree = 0;
}

City::City (uint32_t new_parent) {
    exists = true;

    parent_civilization = new_parent;

    breeding_counter = Game_Constants::BREEDING_RATE;
    captured_counter = 0;

    gather_zone_wheat = 0;
    gather_zone_tree = 0;
}

void City::add_checksum_data (vector<uint32_t>& data) const {
    data.push_back((uint32_t) exists);

    data.push_back(parent_civilization);

    data.push_back(tile.x);
    data.push_back(tile.y);

    for (size_t i = 0; i < people.size(); i++) {
        data.push_back(people[i]);
    }

    data.push_back(breeding_counter);
    data.push_back(captured_counter);

    data.push_back(gather_zone_wheat);
    data.push_back(gather_zone_tree);
}

bool City::get_exists () const {
    return exists;
}

void City::set_exists (bool new_exists) {
    exists = new_exists;
}

uint32_t City::get_parent_civilization () const {
    return parent_civilization;
}

void City::set_parent_civilization (uint32_t new_parent) {
    parent_civilization = new_parent;
}

Coords<uint32_t> City::get_tile () const {
    return tile;
}

void City::set_tile (const Coords<uint32_t>& new_tile) {
    tile = new_tile;
}

const vector<uint32_t>& City::get_people () const {
    return people;
}

void City::add_person (uint32_t person) {
    for (size_t i = 0; i < people.size(); i++) {
        if (people[i] == person) {
            // Exit early, because this person is already in the list
            return;
        }
    }

    people.push_back(person);
}

void City::remove_person (uint32_t person) {
    for (size_t i = 0; i < people.size(); i++) {
        if (people[i] == person) {
            people.erase(people.begin() + i);

            break;
        }
    }
}

int32_t City::get_size () const {
    return int32_t(Game_Constants::BUILDING_SIZE * Game_Constants::TILE_SIZE);
}

int32_t City::get_x () const {
    return int32_t(tile.x * Game_Constants::TILE_SIZE);
}

int32_t City::get_y () const {
    return int32_t(tile.y * Game_Constants::TILE_SIZE);
}

int32_t City::get_center_x () const {
    return int32_t(tile.x * Game_Constants::TILE_SIZE) + get_size() / 2;
}

int32_t City::get_center_y () const {
    return int32_t(tile.y * Game_Constants::TILE_SIZE) + get_size() / 2;
}

uint32_t City::get_region () const {
    return Game::get_chunk(Coords<uint32_t>(get_chunk_x(), get_chunk_y())).get_parent_region();
}

uint32_t City::get_chunk_x () const {
    return tile.x / Game_Constants::CHUNK_SIZE;
}

uint32_t City::get_chunk_y () const {
    return tile.y / Game_Constants::CHUNK_SIZE;
}

uint32_t City::get_tile_x () const {
    return tile.x;
}

uint32_t City::get_tile_y () const {
    return tile.y;
}

Collision_Rect<int32_t> City::get_spawn_zone () const {
    // pixels
    int32_t spawn_zone_x = get_center_x() - get_size() * 4;
    int32_t spawn_zone_y = get_center_y() - get_size() * 4;
    int32_t spawn_zone_width = get_size() * 8;
    int32_t spawn_zone_height = get_size() * 8;

    if (spawn_zone_x < 0) {
        spawn_zone_x = 0;
    }

    if (spawn_zone_y < 0) {
        spawn_zone_y = 0;
    }

    if (spawn_zone_x + spawn_zone_width + (int32_t) Game_Constants::PERSON_SIZE >= Game::get_world_width()) {
        spawn_zone_width = Game::get_world_width() - 1 - spawn_zone_x - (int32_t) Game_Constants::PERSON_SIZE;
    }

    if (spawn_zone_y + spawn_zone_height + (int32_t) Game_Constants::PERSON_SIZE >= Game::get_world_height()) {
        spawn_zone_height = Game::get_world_height() - 1 - spawn_zone_y - (int32_t) Game_Constants::PERSON_SIZE;
    }

    return Collision_Rect<int32_t>(spawn_zone_x, spawn_zone_y, spawn_zone_width, spawn_zone_height);
}

uint32_t City::get_population () const {
    return people.size();
}

bool City::has_maximum_population () const {
    return get_population() >= Game_Constants::CITY_POPULATION_MAX;
}

uint32_t City::get_excess_population () const {
    uint32_t denominator = 100 / Game_Constants::CITY_POPULATION_DESIRED;
    uint32_t minimum_population = Game_Constants::CITY_POPULATION_MAX / denominator;

    if (get_population() > minimum_population) {
        return get_population() - minimum_population;
    } else {
        return 0;
    }
}

bool City::has_excess_population () const {
    return get_excess_population() > 0;
}

bool City::needs_repair () const {
    const Tile& our_tile = Game::get_tile(tile);

    return our_tile.needs_repair();
}

uint32_t City::repair_count_needed () const {
    const Tile& our_tile = Game::get_tile(tile);

    return our_tile.repair_count_needed();
}

bool City::was_recently_captured () const {
    return captured_counter != 0;
}

void City::set_just_captured () {
    captured_counter = Game_Constants::CAPTURE_COOLDOWN;
}

void City::breed (uint32_t index, RNG& rng) {
    if (get_exists()) {
        if (!has_maximum_population()) {
            if (--breeding_counter == 0) {
                breeding_counter = Game_Constants::BREEDING_RATE;

                Collision_Rect<int32_t> box_spawn_zone = get_spawn_zone();
                int32_t x = (int32_t) rng.random_range((uint32_t) box_spawn_zone.x,
                                                       (uint32_t) (box_spawn_zone.x + box_spawn_zone.w));
                int32_t y = (int32_t) rng.random_range((uint32_t) box_spawn_zone.y,
                                                       (uint32_t) (box_spawn_zone.y + box_spawn_zone.h));

                Game::new_people.push_back(Person(index,
                                                  Collision_Rect<int32_t>(x, y, Game_Constants::PERSON_SIZE,
                                                                          Game_Constants::PERSON_SIZE)));
            }
        }
    }
}

void City::capture_cooldown () {
    if (get_exists()) {
        if (captured_counter > 0) {
            captured_counter--;
        }
    }
}

uint32_t City::get_gather_zone_tile_count (Tile::Type tile_type) const {
    if (tile_type == Tile::Type::WHEAT) {
        return gather_zone_wheat;
    } else if (tile_type == Tile::Type::TREE) {
        return gather_zone_tree;
    } else {
        return 0;
    }
}

bool City::allowed_to_update_gather_zone (uint32_t frame, uint32_t index) const {
    if ((frame + (index % Engine::UPDATE_RATE)) % Game_Constants::CITY_GATHER_ZONE_UPDATE_PERIOD == 0) {
        return true;
    } else {
        return false;
    }
}

void City::update_gather_zone (uint32_t frame, uint32_t index) {
    if (get_exists()) {
        if (allowed_to_update_gather_zone(frame, index)) {
            gather_zone_wheat = 0;
            gather_zone_tree = 0;

            vector<Coords<uint32_t>> chunk_coords = Chunk::get_zone_chunk_coords(get_chunk_x(), get_chunk_y(),
                                                                                 Game_Constants::GATHER_ZONE_RANGE);

            for (size_t i = 0; i < chunk_coords.size(); i++) {
                const Chunk& chunk = Game::get_chunk(chunk_coords[i]);

                gather_zone_wheat += chunk.get_tile_count(Tile::Type::WHEAT);
                gather_zone_tree += chunk.get_tile_count(Tile::Type::TREE);
            }
        }
    }
}

void City::write_info_string (string& text) const {
    const Tile& our_tile = Game::get_tile(tile);

    text += "Health: " + Strings::num_to_string(our_tile.get_health()) + "/" +
            Strings::num_to_string(our_tile.get_health_max()) + "\n";

    text += "\n";

    text += "Population: " + Strings::num_to_string(get_population()) + "\n";
}
