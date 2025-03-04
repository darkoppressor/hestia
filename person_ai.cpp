/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "person.h"
#include "game.h"
#include "game_constants.h"

#include <int_math.h>
#include <engine.h>
#include <sorting.h>

#include <utility>
#include <unordered_set>

using namespace std;

void Person::put_our_city_first (vector<uint32_t>& cities) const {
    uint32_t our_city = get_parent_city();

    for (size_t i = 0; i < cities.size(); i++) {
        if (cities[i] == our_city) {
            if (i > 0) {
                swap(cities[i], cities[0]);
            }

            break;
        }
    }
}

void Person::put_our_chunk_first (vector<Coords<uint32_t>>& chunk_coords) const {
    Coords<uint32_t> our_chunk_coords(get_chunk_x(), get_chunk_y());

    for (size_t i = 0; i < chunk_coords.size(); i++) {
        if (chunk_coords[i] == our_chunk_coords) {
            if (i > 0) {
                swap(chunk_coords[i], chunk_coords[0]);
            }

            break;
        }
    }
}

void Person::filter_zone_chunks (vector<Coords<uint32_t>>& chunk_coords,
                                 const vector<Tile::Type>& desired_tile_types) const {
    for (size_t i = 0; i < chunk_coords.size();) {
        const Chunk& chunk = Game::get_chunk(chunk_coords[i]);
        bool chunk_has_desired_tile = false;

        for (size_t n = 0; n < desired_tile_types.size(); n++) {
            if (chunk.get_tile_count(desired_tile_types[n]) > 0) {
                chunk_has_desired_tile = true;

                break;
            }
        }

        if (!chunk_has_desired_tile) {
            chunk_coords.erase(chunk_coords.begin() + i);
        } else {
            i++;
        }
    }
}

bool Person::forage_zone_has_food (const vector<Coords<uint32_t>>& chunk_coords) const {
    for (size_t i = 0; i < chunk_coords.size(); i++) {
        const Chunk& chunk = Game::get_chunk(chunk_coords[i]);

        if (chunk.has_food()) {
            return true;
        }
    }

    return false;
}

void Person::find_tile (RNG& rng, const vector<Coords<uint32_t>>& chunk_coords) {
    for (size_t i = 0; i < chunk_coords.size(); i++) {
        uint32_t tile_x = chunk_coords[i].x * Game_Constants::CHUNK_SIZE;
        uint32_t tile_y = chunk_coords[i].y * Game_Constants::CHUNK_SIZE;

        // A list of valid tile coordinates within the chunk
        vector<Coords<uint32_t>> tile_coords;

        for (uint32_t x = tile_x; x < tile_x + Game_Constants::CHUNK_SIZE; x++) {
            for (uint32_t y = tile_y; y < tile_y + Game_Constants::CHUNK_SIZE; y++) {
                Coords<uint32_t> tile_check_coords(x, y);

                if (Game::tile_exists(tile_check_coords)) {
                    const Tile& tile = Game::get_tile(tile_check_coords);

                    if (tile.is_alive() && tile.get_type() == goal.get_goal_tile_type()) {
                        tile_coords.push_back(tile_check_coords);
                    }
                }
            }
        }

        if (tile_coords.size() > 0) {
            uint32_t random_tile = rng.random_range(0, tile_coords.size() - 1);

            goal.set_coords_tiles(Coords<uint32_t>(tile_coords[random_tile].x, tile_coords[random_tile].y));

            return;
        }
    }

    // If we did not find a valid tile
    abandon_goal();
}

bool Person::allowed_to_select_ai_goal (uint32_t frame, uint32_t our_index) const {
    if ((frame + (our_index % Engine::UPDATE_RATE)) % Game_Constants::AI_GOAL_SELECTION_PERIOD == 0) {
        return true;
    } else {
        return false;
    }
}

void Person::consider_ignoring (vector<AI_Choice>& choices) const {
    if (has_goal()) {
        choices.push_back(AI_Choice(AI_Goal::Type::NONE, Game_Constants::PRIORITY_IGNORE));
    }
}

void Person::consider_gathering (vector<AI_Choice>& choices) const {
    if (goal.gather_can_interrupt()) {
        if (has_inventory_space()) {
            const City& city = Game::get_city(get_parent_city());

            const Civilization& civilization = Game::get_civilization(get_parent_civilization());

            if (city.get_gather_zone_tile_count(Tile::Type::WHEAT) > 0) {
                if (civilization.is_item_needed(Inventory::Item_Type::WHEAT)) {
                    choices.push_back(AI_Choice(AI_Goal::Type::GATHER_WHEAT, Game_Constants::PRIORITY_GATHER));
                } else if (civilization.is_item_desired(Inventory::Item_Type::WHEAT)) {
                    choices.push_back(AI_Choice(AI_Goal::Type::GATHER_WHEAT, Game_Constants::PRIORITY_GATHER_SURPLUS));
                }
            }

            if (city.get_gather_zone_tile_count(Tile::Type::TREE) > 0) {
                if (civilization.is_item_needed(Inventory::Item_Type::TREE)) {
                    choices.push_back(AI_Choice(AI_Goal::Type::GATHER_TREE, Game_Constants::PRIORITY_GATHER));
                } else if (civilization.is_item_desired(Inventory::Item_Type::TREE)) {
                    choices.push_back(AI_Choice(AI_Goal::Type::GATHER_TREE, Game_Constants::PRIORITY_GATHER_SURPLUS));
                }
            }
        }
    }
}

void Person::consider_emptying_inventory (vector<AI_Choice>& choices) const {
    if (goal.empty_inventory_can_interrupt()) {
        if (!has_inventory_space()) {
            choices.push_back(AI_Choice(AI_Goal::Type::EMPTY_INVENTORY, Game_Constants::PRIORITY_EMPTY_INVENTORY_FULL));
        } else if (is_inventory_space_low()) {
            choices.push_back(AI_Choice(AI_Goal::Type::EMPTY_INVENTORY,
                                        Game_Constants::PRIORITY_EMPTY_INVENTORY_ALMOST_FULL));
        } else if (has_item_to_deposit()) {
            choices.push_back(AI_Choice(AI_Goal::Type::EMPTY_INVENTORY, Game_Constants::PRIORITY_EMPTY_INVENTORY));
        }
    }
}

vector<Coords<uint32_t>> Person::consider_eating (vector<AI_Choice>& choices) const {
    vector<Coords<uint32_t>> forage_chunk_coords;

    if (!is_full()) {
        if (has_food()) {
            if (goal.eat_can_interrupt()) {
                choices.push_back(AI_Choice(AI_Goal::Type::EAT, Game_Constants::PRIORITY_EAT));
            }
        } else {
            if (goal.forage_can_interrupt()) {
                forage_chunk_coords = Chunk::get_zone_chunk_coords(get_chunk_x(), get_chunk_y(),
                                                                   Game_Constants::FORAGE_ZONE_RANGE);

                put_our_chunk_first(forage_chunk_coords);

                vector<Tile::Type> desired_tile_types;

                desired_tile_types.push_back(Tile::Type::WHEAT);
                filter_zone_chunks(forage_chunk_coords, desired_tile_types);
            }

            if (forage_zone_has_food(forage_chunk_coords)) {
                choices.push_back(AI_Choice(AI_Goal::Type::FORAGE, Game_Constants::PRIORITY_EAT));
            } else {
                if (goal.eat_at_home_can_interrupt()) {
                    const Civilization& civilization = Game::get_civilization(get_parent_civilization());

                    if (civilization.has_food()) {
                        choices.push_back(AI_Choice(AI_Goal::Type::EAT_AT_HOME, Game_Constants::PRIORITY_EAT));
                    }
                }
            }
        }
    }

    return forage_chunk_coords;
}

Target_Scan_Result Person::target_scan (vector<AI_Choice>& choices, RNG& rng,
                                        const Quadtree<int32_t, uint32_t>& quadtree, uint32_t our_index) const {
    Target_Scan_Result result;

    if (goal.retreat_can_interrupt() || goal.attack_person_melee_can_interrupt() ||
        goal.attack_building_melee_can_interrupt()) {
        vector<uint32_t> targets_people;
        Collision_Rect<int32_t> box_sight = get_sight_box();

        // Only get the quadtree objects if we are retreating or attacking a person
        if (goal.retreat_can_interrupt() || goal.attack_person_melee_can_interrupt()) {
            quadtree.get_objects(targets_people, box_sight);
        }

        vector<AI_Target> valid_targets;

        // Note that scores must never be negative
        int32_t friendly_score = 0;
        int32_t enemy_score = 0;
        unordered_set<uint32_t> collisions;

        for (uint32_t i = 0; i < targets_people.size(); i++) {
            if (!collisions.count(targets_people[i])) {
                collisions.emplace(targets_people[i]);

                if (targets_people[i] != our_index) {
                    const Person& person = Game::get_person(targets_people[i]);

                    if (person.is_alive() && Collision::check_rect(box_sight, person.get_box())) {
                        int32_t person_score = (int32_t) person.get_health() + (int32_t) person.get_attack() * 3 +
                                               (int32_t) person.get_defense() * 2;

                        if (is_friends_with_person(targets_people[i])) {
                            friendly_score += person_score;
                        } else if (is_enemies_with_person(targets_people[i])) {
                            if (could_damage_person(person)) {
                                uint64_t person_distance =
                                    Int_Math::get_distance_between_points_no_sqrt(box.get_center(),
                                                                                  person.get_box().get_center());

                                valid_targets.push_back(AI_Target(person_distance, person.get_health(),
                                                                  person.get_attack(), person.get_defense()));
                                valid_targets.back().set_person_index(targets_people[i]);
                            }

                            enemy_score += person_score;
                        }
                    }
                }
            }
        }

        if (goal.attack_building_melee_can_interrupt()) {
            uint32_t sight_range = Game_Constants::SIGHT_RANGE;
            uint32_t tile_start_x = get_tile_x();
            uint32_t tile_start_y = get_tile_y();

            if (tile_start_x > sight_range) {
                tile_start_x -= sight_range;
            } else {
                tile_start_x = 0;
            }

            if (tile_start_y > sight_range) {
                tile_start_y -= sight_range;
            } else {
                tile_start_y = 0;
            }

            uint32_t tile_end_x = tile_start_x + sight_range * 2;
            uint32_t tile_end_y = tile_start_y + sight_range * 2;

            if (tile_end_x >= Game::get_world_width_tiles()) {
                tile_end_x = Game::get_world_width_tiles() - 1;
            }

            if (tile_end_y >= Game::get_world_height_tiles()) {
                tile_end_y = Game::get_world_height_tiles() - 1;
            }

            for (uint32_t x = tile_start_x; x <= tile_end_x; x++) {
                for (uint32_t y = tile_start_y; y <= tile_end_y; y++) {
                    Coords<uint32_t> tile_coords(x, y);

                    if (Game::tile_exists(tile_coords)) {
                        const Tile& tile = Game::get_tile(tile_coords);

                        if (tile.is_alive() && tile.is_building()) {
                            int32_t building_score = (int32_t) tile.get_health() + (int32_t) tile.get_defense() * 2;

                            if (is_friends_with_tile(tile_coords)) {
                                friendly_score += building_score;
                            } else if (is_enemies_with_tile(tile_coords)) {
                                if (could_damage_tile(tile)) {
                                    uint64_t building_distance =
                                        Int_Math::get_distance_between_points_no_sqrt(box.get_center(),
                                                                                      Coords<int32_t>(tile.get_center_x(
                                                                                                          x),
                                                                                                      tile.get_center_y(
                                                                                                          y)));

                                    valid_targets.push_back(AI_Target(building_distance, tile.get_health(), 0,
                                                                      tile.get_defense()));
                                    valid_targets.back().set_building_tile_coords(tile_coords);
                                }

                                enemy_score += building_score;
                            }
                        }
                    }
                }
            }
        }

        // Score advantage will be one of:
        // 1 if there is no advantage or disadvantage
        // Greater than 1 if there is an advantage, with the number representing the ratio of friendly to enemy score
        // Less than -1 if there is a disadvantage, with the (absolute value of the) number representing the ratio of
        // enemy to friendly score
        int32_t score_advantage = 1;

        if (enemy_score > 0 && friendly_score > enemy_score) {
            score_advantage = friendly_score / enemy_score;
        } else if (friendly_score > 0 && friendly_score < enemy_score) {
            score_advantage = -enemy_score / friendly_score;

            // If the score ratio is negative but rounds to -1, just call it 1 (meaning no advantage or disadvantage)
            if (score_advantage == -1) {
                score_advantage = 1;
            }
        }

        if (!goal.attack_person_melee_can_interrupt()) {
            // Remove all of the target people
            for (size_t i = 0; i < valid_targets.size();) {
                if (valid_targets[i].is_person()) {
                    valid_targets.erase(valid_targets.begin() + i);
                } else {
                    i++;
                }
            }
        }

        if (!goal.attack_building_melee_can_interrupt()) {
            // Remove all of the target buildings
            for (size_t i = 0; i < valid_targets.size();) {
                if (!valid_targets[i].is_person()) {
                    valid_targets.erase(valid_targets.begin() + i);
                } else {
                    i++;
                }
            }
        }

        if ((goal.attack_person_melee_can_interrupt() || goal.attack_building_melee_can_interrupt()) &&
            !home_was_recently_captured() && valid_targets.size() > 0) {
            Sorting::quick_sort(valid_targets);

            uint32_t valid_target_index = rng.weighted_random_range(0, valid_targets.size() - 1,
                                                                    valid_targets.size() - 1, RNG::Weight::NORMAL);
            int32_t priority_attack = 0;

            if (score_advantage >= Game_Constants::AI_COMBAT_SCORE_RATIO_OVERWHELMING) {
                priority_attack = Game_Constants::PRIORITY_ATTACK_MELEE_WITH_OVERWHELMING_ADVANTAGE;
            } else if (score_advantage >= Game_Constants::AI_COMBAT_SCORE_RATIO_MAJOR) {
                priority_attack = Game_Constants::PRIORITY_ATTACK_MELEE_WITH_MAJOR_ADVANTAGE;
            } else if (score_advantage > 1) {
                priority_attack = Game_Constants::PRIORITY_ATTACK_MELEE_WITH_MINOR_ADVANTAGE;
            } else {
                priority_attack = Game_Constants::PRIORITY_ATTACK_MELEE_WITH_NO_ADVANTAGE;
            }

            if (valid_targets[valid_target_index].is_person()) {
                result.person_index = valid_targets[valid_target_index].person_index;

                choices.push_back(AI_Choice(AI_Goal::Type::ATTACK_PERSON_MELEE, priority_attack));
            } else {
                result.building_tile_coords = valid_targets[valid_target_index].building_tile_coords;

                choices.push_back(AI_Choice(AI_Goal::Type::ATTACK_BUILDING_MELEE, priority_attack));
            }
        }

        if (goal.retreat_can_interrupt()) {
            int32_t priority_retreat = 0;

            if (health_low()) {
                priority_retreat += Game_Constants::PRIORITY_RETREAT_WITH_LOW_HEALTH;
            }

            // If we have a score disadvantage
            if (score_advantage < -1) {
                int32_t score_ratio = -score_advantage;

                if (score_ratio >= Game_Constants::AI_COMBAT_SCORE_RATIO_OVERWHELMING) {
                    priority_retreat += Game_Constants::PRIORITY_RETREAT_WITH_OVERWHELMING_DISADVANTAGE;
                } else if (score_ratio >= Game_Constants::AI_COMBAT_SCORE_RATIO_MAJOR) {
                    priority_retreat += Game_Constants::PRIORITY_RETREAT_WITH_MAJOR_DISADVANTAGE;
                } else {
                    priority_retreat += Game_Constants::PRIORITY_RETREAT_WITH_MINOR_DISADVANTAGE;
                }
            }

            if (priority_retreat > 0) {
                const City& city = Game::get_city(get_parent_city());

                if (Int_Math::get_distance_between_points_no_sqrt(Coords<int32_t>((int32_t) get_tile_x(),
                                                                                  (int32_t) get_tile_y()),
                                                                  Coords<int32_t>((int32_t) city.get_tile_x(),
                                                                                  (int32_t) city.get_tile_y())) >
                    Game_Constants::HOME_DEFENSE_RANGE) {
                    choices.push_back(AI_Choice(AI_Goal::Type::RETREAT, priority_retreat));
                }
            }
        }
    }

    return result;
}

Coords<uint32_t> Person::consider_building (vector<AI_Choice>& choices) const {
    if (goal.build_can_interrupt()) {
        const Civilization& civilization = Game::get_civilization(get_parent_civilization());
        vector<Coords<uint32_t>> unfinished_buildings = civilization.get_unfinished_buildings();

        if (unfinished_buildings.size() > 0 && has_build_material()) {
            bool some_building_looked_at = false;
            uint64_t nearest_unfinished_building = 0;
            uint32_t nearest_building_index = 0;

            for (size_t i = 0; i < unfinished_buildings.size(); i++) {
                // If this unfinished building is not flagged
                if (!civilization.get_unfinished_building_flag(unfinished_buildings[i])) {
                    uint64_t distance_to_building = Int_Math::get_distance_between_points_no_sqrt(box.get_center(),
                                                                                                  Coords<int32_t>(
                                                                                                      Tile::get_center_x
                                                                                                      (
                                                                                                          unfinished_buildings
                                                                                                          [i].x,
                                                                                                          Tile::
                                                                                                          get_tile_type_size
                                                                                                              (Tile::
                                                                                                              Type::
                                                                                                              BUILDING_UNFINISHED)),
                                                                                                      Tile::get_center_y
                                                                                                      (
                                                                                                          unfinished_buildings
                                                                                                          [i].y,
                                                                                                          Tile::
                                                                                                          get_tile_type_size
                                                                                                              (Tile::
                                                                                                              Type::
                                                                                                              BUILDING_UNFINISHED))));

                    if (!some_building_looked_at || distance_to_building < nearest_unfinished_building) {
                        some_building_looked_at = true;

                        nearest_unfinished_building = distance_to_building;

                        nearest_building_index = (uint32_t) i;
                    }
                }
            }

            if (some_building_looked_at) {
                choices.push_back(AI_Choice(AI_Goal::Type::BUILD, Game_Constants::PRIORITY_BUILD));

                return unfinished_buildings[nearest_building_index];
            }
        }
    }

    return Coords<uint32_t>(0, 0);
}

void Person::consider_repairing (vector<AI_Choice>& choices) const {
    if (goal.repair_can_interrupt()) {
        const City& city = Game::get_city(get_parent_city());

        if (city.needs_repair() && has_repair_material()) {
            choices.push_back(AI_Choice(AI_Goal::Type::REPAIR, Game_Constants::PRIORITY_REPAIR));
        }
    }
}

void Person::set_new_goal (RNG& rng, AI_Goal::Type new_goal_type, Target_Scan_Result target_scan_result,
                           vector<Coords<uint32_t>> forage_chunk_coords, Coords<uint32_t> unfinished_building_coords) {
    // If we had the building goal before, we are interrupting it
    if (goal.is_build()) {
        // Unflag the target building from our previous goal
        Game::set_civilization_unfinished_building_flag(get_parent_civilization(), goal.get_coords_tiles(), false);
    }

    goal.set_type(new_goal_type);

    if (goal.is_gather()) {
        vector<Coords<uint32_t>> gather_chunk_coords;

        const City& city = Game::get_city(get_parent_city());

        if (city.get_gather_zone_tile_count(goal.get_goal_tile_type()) > 0) {
            gather_chunk_coords = Chunk::get_zone_chunk_coords(city.get_chunk_x(), city.get_chunk_y(),
                                                               Game_Constants::GATHER_ZONE_RANGE);

            Sorting::shuffle(rng, gather_chunk_coords);

            put_our_chunk_first(gather_chunk_coords);

            vector<Tile::Type> desired_tile_types;

            desired_tile_types.push_back(goal.get_goal_tile_type());
            filter_zone_chunks(gather_chunk_coords, desired_tile_types);
        }

        find_tile(rng, gather_chunk_coords);
    } else if (goal.is_empty_inventory()) {
        const City& city = Game::get_city(get_parent_city());

        goal.set_coords_tiles(Coords<uint32_t>(city.get_tile().x, city.get_tile().y));
    } else if (goal.is_eat()) {
        goal.set_coords_pixels(Coords<int32_t>(box.center_x(), box.center_y()));
    } else if (goal.is_eat_at_home()) {
        const City& city = Game::get_city(get_parent_city());

        goal.set_coords_tiles(Coords<uint32_t>(city.get_tile().x, city.get_tile().y));
    } else if (goal.is_forage()) {
        find_tile(rng, forage_chunk_coords);
    } else if (goal.is_retreat()) {
        const Civilization& civilization = Game::get_civilization(get_parent_civilization());
        vector<uint32_t> civ_cities = civilization.get_cities();

        // If we have multiple cities
        if (civ_cities.size() > 1) {
            put_our_city_first(civ_cities);

            uint32_t city_index = rng.weighted_random_range(0, civ_cities.size() - 1, 0, RNG::Weight::NORMAL);

            const City& city = Game::get_city(city_index);

            goal.set_coords_pixels(Coords<int32_t>(city.get_center_x(), city.get_center_y()));
        }
        // If we have only one city
        else {
            if (rng.random_range(0, 99) < Game_Constants::RETREAT_HOME_CHANCE) {
                const City& city = Game::get_city(get_parent_city());

                goal.set_coords_pixels(Coords<int32_t>(city.get_center_x(), city.get_center_y()));
            } else {
                int32_t retreat_zone_x = box.center_x() -
                                         (Game_Constants::RETREAT_ZONE_RANGE * (int32_t) Game_Constants::TILE_SIZE);
                int32_t retreat_zone_y = box.center_y() -
                                         (Game_Constants::RETREAT_ZONE_RANGE * (int32_t) Game_Constants::TILE_SIZE);
                int32_t retreat_zone_width = Game_Constants::RETREAT_ZONE_RANGE * (int32_t) Game_Constants::TILE_SIZE *
                                             2;
                int32_t retreat_zone_height = Game_Constants::RETREAT_ZONE_RANGE * (int32_t) Game_Constants::TILE_SIZE *
                                              2;

                if (retreat_zone_x < 0) {
                    retreat_zone_x = 0;
                }

                if (retreat_zone_y < 0) {
                    retreat_zone_y = 0;
                }

                if (retreat_zone_x + retreat_zone_width >= Game::get_world_width()) {
                    retreat_zone_width = Game::get_world_width() - 1 - retreat_zone_x;
                }

                if (retreat_zone_y + retreat_zone_height >= Game::get_world_height()) {
                    retreat_zone_height = Game::get_world_height() - 1 - retreat_zone_y;
                }

                int32_t x = (int32_t) rng.random_range((uint32_t) retreat_zone_x,
                                                       uint32_t(retreat_zone_x + retreat_zone_width));
                int32_t y = (int32_t) rng.random_range((uint32_t) retreat_zone_y,
                                                       uint32_t(retreat_zone_y + retreat_zone_height));

                goal.set_coords_pixels(Coords<int32_t>(x, y));
            }
        }
    } else if (goal.is_attack_person_melee()) {
        goal.set_person_index(target_scan_result.person_index);
    } else if (goal.is_attack_building_melee()) {
        goal.set_coords_tiles(target_scan_result.building_tile_coords);
    } else if (goal.is_build()) {
        goal.set_coords_tiles(unfinished_building_coords);

        Game::set_civilization_unfinished_building_flag(get_parent_civilization(), unfinished_building_coords, true);
    } else if (goal.is_repair()) {
        const City& city = Game::get_city(get_parent_city());

        goal.set_coords_tiles(Coords<uint32_t>(city.get_tile().x, city.get_tile().y));
    }
}

void Person::ai (RNG& rng, const Quadtree<int32_t, uint32_t>& quadtree, uint32_t frame, uint32_t our_index) {
    if (is_alive()) {
        if (allowed_to_select_ai_goal(frame, our_index)) {
            vector<AI_Choice> choices;

            consider_ignoring(choices);

            consider_emptying_inventory(choices);

            vector<Coords<uint32_t>> forage_chunk_coords = consider_eating(choices);
            Coords<uint32_t> unfinished_building_coords(0, 0);

            if (!home_was_recently_captured()) {
                consider_gathering(choices);

                unfinished_building_coords = consider_building(choices);

                consider_repairing(choices);
            }

            Target_Scan_Result target_scan_result = target_scan(choices, rng, quadtree, our_index);

            if (choices.size() > 0) {
                Sorting::quick_sort(choices);

                uint32_t choice_index = rng.weighted_random_range(0, choices.size() - 1, choices.size() - 1,
                                                                  RNG::Weight::WEAK);

                if (choices[choice_index].goal_type != AI_Goal::Type::NONE) {
                    set_new_goal(rng, choices[choice_index].goal_type, target_scan_result, forage_chunk_coords,
                                 unfinished_building_coords);
                }
            }
        }

        if (has_goal()) {
            if (goal_within_range()) {
                brake();

                if (goal.is_first_count() && !is_goal_valid()) {
                    abandon_goal();
                } else {
                    if (goal.countdown()) {
                        complete_goal(rng);
                    }
                }
            } else {
                // If the goal hasn't counted down any yet
                if (goal.is_first_count()) {
                    force += Int_Vector(get_move_force(), get_angle_to_goal());
                }
                // If the goal has counted down at least once, a goal we reached and started counting down is no longer
                // in range
                else {
                    abandon_goal();
                }
            }
        } else {
            brake();
        }
    }
}

Target_Scan_Result::Target_Scan_Result () {
    person_index = 0;
    building_tile_coords.x = 0;
    building_tile_coords.y = 0;
}

AI_Target::AI_Target (uint64_t new_distance, int16_t new_health, int16_t new_attack, int16_t new_defense) {
    person = true;

    person_index = 0;

    distance = new_distance;
    health = new_health;
    attack = new_attack;
    defense = new_defense;
}

bool AI_Target::is_person () const {
    return person;
}

void AI_Target::set_person_index (uint32_t new_person_index) {
    person_index = new_person_index;

    person = true;
}

void AI_Target::set_building_tile_coords (const Coords<uint32_t>& new_building_tile_coords) {
    building_tile_coords = new_building_tile_coords;

    person = false;
}

bool AI_Target::operator<= (const AI_Target& target) const {
    uint32_t score = 0;

    if (health >= target.health) {
        score += Game_Constants::AI_TARGET_SELECTION_WEIGHT_HEALTH;
    }

    if (attack >= target.attack) {
        score += Game_Constants::AI_TARGET_SELECTION_WEIGHT_ATTACK;
    }

    if (defense >= target.defense) {
        score += Game_Constants::AI_TARGET_SELECTION_WEIGHT_DEFENSE;
    }

    if (distance >= target.distance) {
        score += Game_Constants::AI_TARGET_SELECTION_WEIGHT_DISTANCE;
    }

    if (score >= Game_Constants::AI_TARGET_SELECTION_WEIGHT_NEEDED) {
        return true;
    } else {
        return false;
    }
}
