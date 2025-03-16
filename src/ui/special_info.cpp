/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "../game/game.h"

#include <ui/special_info.h>
#include <log.h>
#include <data/object_manager.h>
#include <network/network_engine.h>

using namespace std;

string Special_Info::get_special_info_text (string special_info) {
    string text = "";

    if (special_info.length() > 0) {
        if (special_info == "configure_command") {
            Object_Manager::output_command_configuration_info(text);
        } else if (special_info == "person") {
            Game_Selection selection = Game::get_selection();

            if (selection.type == Game_Selection::Type::PERSON) {
                const Person& person = Game::get_person(selection.index);

                person.write_info_string(text);
            }
        } else if (special_info == "city") {
            Game_Selection selection = Game::get_selection();

            if (selection.type == Game_Selection::Type::CITY) {
                const City& city = Game::get_city(selection.index);

                city.write_info_string(text);
            }
        } else if (special_info == "unfinished_building") {
            Game_Selection selection = Game::get_selection();

            if (selection.type == Game_Selection::Type::UNFINISHED_BUILDING) {
                if (Game::tile_exists(selection.tile_coordinates)) {
                    const Tile& tile = Game::get_tile(selection.tile_coordinates);

                    tile.write_info_string(text);
                }
            }
        } else if (special_info == "civilization") {
            Game_Selection selection = Game::get_selection();

            if (selection.type == Game_Selection::Type::CIVILIZATION) {
                const Civilization& civilization = Game::get_civilization(selection.index);

                civilization.write_info_string(text);
            }
        } else if (special_info == "game_over") {
            if (Game::started) {
                Game_Over game_over = Game::get_game_over();
                int32_t leader_index = Game::get_our_leader();

                if (leader_index >= 0) {
                    if (leader_index == game_over.winning_leader) {
                        text += "Victory!\n";
                    } else {
                        text += "Defeat!\n";
                    }
                } else {
                    text += "Game Over!\n";
                }

                text += "\n";

                const Leader& leader = Game::get_leader(game_over.winning_leader);

                if (leader.is_player_controlled()) {
                    uint32_t player_index = leader.get_parent_player();
                    vector<Client_Data*> players = Network_Engine::get_players();

                    text += players[player_index]->name;
                } else {
                    text += "Computer";
                }

                text += " has won a ";

                if (game_over.victory_condition == Game_Over::Victory_Condition::CONQUEST) {
                    text += "conquest";
                }

                text += " victory\n";
            }
        } else {
            Log::add_error("Invalid special info text: '" + special_info + "'");
        }
    }

    return text;
}

string Special_Info::get_special_info_sprite (string special_info) {
    string str_sprite_name = "";

    if (special_info.length() > 0) {
        if (special_info == "example") {
            str_sprite_name = "";
        } else {
            Log::add_error("Invalid special info sprite: '" + special_info + "'");
        }
    }

    return str_sprite_name;
}
