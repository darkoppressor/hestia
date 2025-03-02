/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_options.h"

#include <game_manager.h>
#include <network_engine.h>
#include <network_server.h>
#include <engine.h>
#include <object_manager.h>
#include <options.h>
#include <engine_input.h>
#include <window_manager.h>
#include <engine_data.h>
#include <math_vector.h>
#include <game_window.h>
#include <engine_strings.h>

using namespace std;

void Game_Manager::handle_drag_and_drop (string file) {
    // Do something with file
}

void Game_Manager::prepare_for_input () {
    if (in_progress) {
        command_states.clear();

        display_scoreboard = false;

        if (Game::started) {
            if (Game::is_game_over()) {
                Game_Manager::paused = true;
            }

            if (Game::get_selection().type == Game_Selection::Type::PERSON &&
                !Window_Manager::get_window("game_person")->on) {
                Game::clear_selection();
            }

            if (Game::get_selection().type == Game_Selection::Type::CITY &&
                !Window_Manager::get_window("game_city")->on) {
                Game::clear_selection();
            }

            if (Game::get_selection().type == Game_Selection::Type::UNFINISHED_BUILDING &&
                !Window_Manager::get_window("game_unfinished_building")->on) {
                Game::clear_selection();
            }

            if (Game::get_selection().type == Game_Selection::Type::CIVILIZATION &&
                !Window_Manager::get_window("game_civilization")->on) {
                Game::clear_selection();
            }

            // Reset the camera speeds to their defaults
            camera_speed = Options::camera_speed;
            Game::edge_scroll_speed = Game_Options::edge_scroll_speed;
        }
    }
}

void Game_Manager::handle_command_states_multiplayer () {
    if (in_progress && Game::started) {
        if (Network_Engine::status == "server") {
            Network_Server::prepare_server_input_states();

            for (size_t i = 0; i < Network_Engine::clients.size(); i++) {
                if (!paused) {
                    // Example multiplayer command state
                    /**if(Network_Engine::clients[i].game_command_state("some_command")){
                        // Deal with command state here
                       }*/
                }
            }
        }
    }
}

void Game_Manager::handle_game_commands_multiplayer () {
    if (in_progress && Game::started) {
        if (Network_Engine::status == "server") {
            for (size_t i = 0; i < Network_Engine::clients.size(); i++) {
                for (size_t j = 0; j < Network_Engine::clients[i].command_buffer.size(); j++) {
                    string command_name = Network_Engine::clients[i].command_buffer[j];

                    if (!paused) {
                        // Example multiplayer command
                        /**if(command_name=="some_command"){
                            // Deal with command here
                           }*/
                    }
                }

                Network_Engine::clients[i].command_buffer.clear();
            }
        }
    }
}

void Game_Manager::handle_input_states_gui () {
    int mouse_x = 0;
    int mouse_y = 0;

    Engine::get_mouse_state(&mouse_x, &mouse_y);

    if (in_progress && Game::started) {
        if (Object_Manager::game_command_state("scoreboard")) {
            display_scoreboard = true;
        }

        // Move the camera via directional keys
        if (Game::move_input_state("left")) {
            cam_state = "left";
        }

        if (Game::move_input_state("up")) {
            cam_state = "up";
        }

        if (Game::move_input_state("right")) {
            cam_state = "right";
        }

        if (Game::move_input_state("down")) {
            cam_state = "down";
        }

        if (Game::move_input_state("left") && Game::move_input_state("up")) {
            cam_state = "left_up";
        }

        if (Game::move_input_state("up") && Game::move_input_state("right")) {
            cam_state = "right_up";
        }

        if (Game::move_input_state("right") && Game::move_input_state("down")) {
            cam_state = "right_down";
        }

        if (Game::move_input_state("down") && Game::move_input_state("left")) {
            cam_state = "left_down";
        }

        if (Game::move_input_state("left") && Game::move_input_state("right")) {
            cam_state = "left";
        }

        if (Game::move_input_state("up") && Game::move_input_state("down")) {
            cam_state = "up";
        }

        if (Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("right")) {
            cam_state = "left_up";
        }

        if (Game::move_input_state("left") && Game::move_input_state("down") && Game::move_input_state("right")) {
            cam_state = "left_down";
        }

        if (Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("down")) {
            cam_state = "left_up";
        }

        if (Game::move_input_state("up") && Game::move_input_state("right") && Game::move_input_state("down")) {
            cam_state = "right_up";
        }

        if (Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("right") &&
            Game::move_input_state("down")) {
            cam_state = "left_up";
        }

        if (!Game::move_input_state("left") && !Game::move_input_state("up") && !Game::move_input_state("right") &&
            !Game::move_input_state("down")) {
            cam_state = "none";
        }

        // Move the camera via edge scrolling
        if (Game::edge_scroll_input_state("left")) {
            Game::edge_scroll_state = "left";
        }

        if (Game::edge_scroll_input_state("up")) {
            Game::edge_scroll_state = "up";
        }

        if (Game::edge_scroll_input_state("right")) {
            Game::edge_scroll_state = "right";
        }

        if (Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "down";
        }

        if (Game::edge_scroll_input_state("left") && Game::edge_scroll_input_state("up")) {
            Game::edge_scroll_state = "left_up";
        }

        if (Game::edge_scroll_input_state("up") && Game::edge_scroll_input_state("right")) {
            Game::edge_scroll_state = "right_up";
        }

        if (Game::edge_scroll_input_state("right") && Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "right_down";
        }

        if (Game::edge_scroll_input_state("down") && Game::edge_scroll_input_state("left")) {
            Game::edge_scroll_state = "left_down";
        }

        if (Game::edge_scroll_input_state("left") && Game::edge_scroll_input_state("right")) {
            Game::edge_scroll_state = "left";
        }

        if (Game::edge_scroll_input_state("up") && Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "up";
        }

        if (Game::edge_scroll_input_state("left") && Game::edge_scroll_input_state("up") &&
            Game::edge_scroll_input_state("right")) {
            Game::edge_scroll_state = "left_up";
        }

        if (Game::edge_scroll_input_state("left") && Game::edge_scroll_input_state("down") &&
            Game::edge_scroll_input_state("right")) {
            Game::edge_scroll_state = "left_down";
        }

        if (Game::edge_scroll_input_state("left") && Game::edge_scroll_input_state("up") &&
            Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "left_up";
        }

        if (Game::edge_scroll_input_state("up") && Game::edge_scroll_input_state("right") &&
            Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "right_up";
        }

        if (Game::edge_scroll_input_state("left") && Game::edge_scroll_input_state("up") &&
            Game::edge_scroll_input_state("right") && Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "left_up";
        }

        if (!Game::edge_scroll_input_state("left") && !Game::edge_scroll_input_state("up") &&
            !Game::edge_scroll_input_state("right") && !Game::edge_scroll_input_state("down")) {
            Game::edge_scroll_state = "none";
        }

        if (cam_state != "none" || (Game_Options::edge_scrolling && Game::edge_scroll_state != "none")) {
            if (Game::get_selection().follow) {
                Game::toggle_selection_follow();
            }
        }

        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if (Game::camera_drag_minimap) {
                Game::center_camera_on_minimap_position();
            }
        }

        // Handle camera speed modifiers
        if (Object_Manager::game_command_state("camera_speed_mod_fast_1") ||
            Object_Manager::game_command_state("camera_speed_mod_fast_2")) {
            camera_speed *= Game_Options::camera_speed_mod_fast;
            Game::edge_scroll_speed *= Game_Options::edge_scroll_speed_mod_fast;
        }

        if (Object_Manager::game_command_state("camera_speed_mod_slow_1") ||
            Object_Manager::game_command_state("camera_speed_mod_slow_2")) {
            camera_speed *= Game_Options::camera_speed_mod_slow;
            Game::edge_scroll_speed *= Game_Options::edge_scroll_speed_mod_slow;
        }
    }
}

void Game_Manager::handle_input_states () {
    int mouse_x = 0;
    int mouse_y = 0;

    Engine::get_mouse_state(&mouse_x, &mouse_y);

    if (in_progress && Game::started) {
        if (!paused) {
            // Example multiplayer command state
            /**if(Object_Manager::game_command_state("some_command")){
                command_states.push_back("some_command");
               }*/
        }
    }
}

bool Game_Manager::handle_game_command_gui (string command_name) {
    if (Game::started) {
        // Multiplayer pause
        if (command_name == "pause") {
            if (Network_Engine::status == "server" && !Game::is_game_over()) {
                toggle_pause();

                Network_Server::send_paused();
            }

            return true;
        } else if (command_name == "build_city") {
            if (Game::get_order() != Game_Order::Type::BUILD_CITY) {
                Game::set_order(Game_Order::Type::BUILD_CITY);
            } else {
                Game::clear_order();
            }

            return true;
        } else if (command_name == "select_civilization") {
            if (Game::get_selection().type != Game_Selection::Type::CIVILIZATION) {
                int32_t leader_index = Game::get_our_leader();

                if (leader_index >= 0) {
                    const Leader& leader = Game::get_leader((uint32_t) leader_index);

                    Game::set_selection(Game_Selection::Type::CIVILIZATION, leader.get_civilization());
                }
            } else {
                Game::clear_selection();
            }

            return true;
        } else if (command_name == "follow_selection") {
            Game::toggle_selection_follow();

            return true;
        } else if (command_name == "minimap_show_resources") {
            Game_Options::set_option("cl_minimap_show_resources",
                                     Strings::bool_to_string(!Game_Options::minimap_show_resources));

            return true;
        }
    }

    // Toggle chat box
    if (command_name == "chat") {
        Engine::chat.toggle_on();

        return true;
    }

    return false;
}

bool Game_Manager::handle_game_command (string command_name) {
    const uint8_t* keystates = SDL_GetKeyboardState(NULL);

    // DEV COMMANDS
    if (Options::dev && keystates[SDL_SCANCODE_F1]) {
        // Example dev command
        /**if(command_name=="some_dev_command"){
            // Dev command here.

            return true;
           }*/
    }

    // END OF DEV COMMANDS

    if (!paused) {
        // Example command
        /**if(command_name=="some_command"){
            // Command here

            return true;
           }*/

        // Example multiplayer command input
        /**if(command_name=="some_command"){
            Network_Engine::add_command(command_name);

            return true;
           }*/
    }

    return false;
}

bool Game_Manager::handle_input_events_gui () {
    bool event_consumed = false;

    if (in_progress) {
        const vector<Game_Command>& game_commands = Object_Manager::get_game_commands();

        for (size_t i = 0; i < game_commands.size() && !event_consumed; i++) {
            if ((Engine_Input::event.type == SDL_CONTROLLERBUTTONDOWN &&
                 game_commands[i].button == Engine_Input::event.cbutton.button) ||
                (Engine_Input::event.type == SDL_KEYDOWN && Engine_Input::event.key.repeat == 0 &&
                 game_commands[i].key == Engine_Input::event.key.keysym.scancode)) {
                event_consumed = handle_game_command_gui(game_commands[i].name);
            }
        }

        if (Game::started) {
            if (Engine_Input::event.type == SDL_MOUSEWHEEL) {
                if (Game::is_mouse_over_minimap()) {
                    if (!event_consumed && Engine_Input::event.wheel.y > 0) {
                        Game::center_camera_on_minimap_position();

                        zoom_camera_in(camera);

                        event_consumed = true;
                    } else if (!event_consumed && Engine_Input::event.wheel.y < 0) {
                        zoom_camera_out(camera);

                        event_consumed = true;
                    }
                } else {
                    if (!event_consumed && Engine_Input::event.wheel.y > 0) {
                        if (Game_Options::zoom_to_cursor) {
                            // pixels
                            Coords<int32_t> mouse_position = Game::get_mouse_coords_pixels();
                            Collision_Rect<double> box_mouse((double) mouse_position.x * Game_Manager::camera_zoom,
                                                             (double) mouse_position.y * Game_Manager::camera_zoom, 1.0,
                                                             1.0);
                            Collision_Rect<double> box_camera(Game_Manager::camera.center_x(),
                                                              Game_Manager::camera.center_y(), 1.0, 1.0);
                            int screen_width = 0;
                            int screen_height = 0;

                            Game_Window::get_renderer_output_size(&screen_width, &screen_height);

                            double screen_distance = ((double) screen_width + (double) screen_height) / 4.0;
                            double magnitude = 50.0 /
                                               (Game_Manager::camera_zoom /
                                                (Math::get_distance_between_points(Coords<double>(box_camera.x,
                                                                                                  box_camera.y),
                                                                                   Coords<double>(box_mouse.x,
                                                                                                  box_mouse.y)) /
                                                 screen_distance));
                            Vector vector_camera(magnitude, box_camera.get_angle_to_rect(box_mouse));
                            Vector_Components vc = vector_camera.get_components();

                            box_camera.x += vc.a;
                            box_camera.y += vc.b;

                            zoom_camera_in(Collision_Rect<double>(box_camera.x, box_camera.y, 0.0, 0.0));
                        } else {
                            zoom_camera_in(camera);
                        }

                        event_consumed = true;
                    } else if (!event_consumed && Engine_Input::event.wheel.y < 0) {
                        zoom_camera_out(camera);

                        event_consumed = true;
                    }
                }
            } else if (Engine_Input::event.type == SDL_MOUSEMOTION) {
                if (!event_consumed && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    if (Game::camera_drag_main) {
                        camera.x -= Engine_Input::event.motion.xrel;
                        camera.y -= Engine_Input::event.motion.yrel;
                    }

                    event_consumed = true;
                }
            } else if (Engine_Input::event.type == SDL_MOUSEBUTTONUP) {
                // We do NOT check for !event_consumed here as we would for most events
                // See the explanation in the comment below (in this if statement) for more information
                if (Engine_Input::event.button.button == SDL_BUTTON_LEFT) {
                    Game::camera_drag_main = false;
                    Game::camera_drag_minimap = false;

                    // We do NOT set event_consumed to true here because we are just "borrowing" this event
                    // We want to see this mouse button up event even if it is used by a window,
                    // because either way we should stop camera dragging if we were previously
                }
            } else if (Engine_Input::event.type == SDL_MOUSEBUTTONDOWN) {
                if (!Window_Manager::is_mouse_over_open_window()) {
                    if (!event_consumed && Engine_Input::event.button.button == SDL_BUTTON_LEFT) {
                        if (Game::is_mouse_over_minimap()) {
                            Game::center_camera_on_minimap_position();

                            Game::camera_drag_minimap = true;
                        } else {
                            Game::camera_drag_main = true;

                            if (Game::has_order()) {
                                int32_t leader = Game::get_our_leader();

                                if (leader >= 0) {
                                    Game::issue_order(Game_Order(Game::get_order(), Game::get_mouse_coords_tiles(),
                                                                 (uint32_t) leader));
                                }
                            } else {
                                // Check for clicks on game objects that can be selected

                                int32_t leader_index = Game::get_our_leader();

                                if (leader_index >= 0) {
                                    const Leader& leader = Game::get_leader((uint32_t) leader_index);

                                    // pixels
                                    Coords<int32_t> mouse_position = Game::get_mouse_coords_pixels();
                                    Collision_Rect<int32_t> box(mouse_position.x, mouse_position.y,
                                                                (int32_t) Engine_Data::cursor_width,
                                                                (int32_t) Engine_Data::cursor_height);
                                    bool person_found = false;

                                    for (uint32_t i = 0; i < Game::get_people_count(); i++) {
                                        const Person& person = Game::get_person(i);

                                        if (person.is_alive() &&
                                            person.get_parent_civilization() == leader.get_civilization() &&
                                            Collision::check_rect(box, person.get_box())) {
                                            person_found = true;
                                            Game::set_selection(Game_Selection::Type::PERSON, i);

                                            break;
                                        }
                                    }

                                    if (!person_found) {
                                        // tiles
                                        Coords<uint32_t> mouse_position_tiles = Game::get_mouse_coords_tiles();

                                        // tiles
                                        uint32_t mouse_size = 1;
                                        uint32_t tile_start_x = mouse_position_tiles.x;
                                        uint32_t tile_start_y = mouse_position_tiles.y;

                                        if (tile_start_x > mouse_size) {
                                            tile_start_x -= mouse_size;
                                        } else {
                                            tile_start_x = 0;
                                        }

                                        if (tile_start_y > mouse_size) {
                                            tile_start_y -= mouse_size;
                                        } else {
                                            tile_start_y = 0;
                                        }

                                        uint32_t tile_end_x = tile_start_x + mouse_size * 2;
                                        uint32_t tile_end_y = tile_start_y + mouse_size * 2;

                                        if (tile_end_x >= Game::get_world_width_tiles()) {
                                            tile_end_x = Game::get_world_width_tiles() - 1;
                                        }

                                        if (tile_end_y >= Game::get_world_height_tiles()) {
                                            tile_end_y = Game::get_world_height_tiles() - 1;
                                        }

                                        bool tile_found = false;

                                        for (uint32_t x = tile_start_x; x <= tile_end_x && !tile_found; x++) {
                                            for (uint32_t y = tile_start_y; y <= tile_end_y && !tile_found; y++) {
                                                Coords<uint32_t> tile_coords(x, y);

                                                if (Game::tile_exists(tile_coords)) {
                                                    const Tile& tile = Game::get_tile(tile_coords);

                                                    if (tile.is_alive()) {
                                                        if (tile.get_type() == Tile::Type::BUILDING_CITY) {
                                                            // If the tile is of type BUILDING_CITY, its parent is a
                                                            // City
                                                            const City& city = Game::get_city(tile.get_parent());

                                                            if (city.get_exists()) {
                                                                Collision_Rect<int32_t> box_tile(tile.get_x(x),
                                                                                                 tile.get_x(y),
                                                                                                 tile.get_size(),
                                                                                                 tile.get_size());

                                                                // If the city is a member of our civilization
                                                                if (city.get_parent_civilization() ==
                                                                    leader.get_civilization() &&
                                                                    Collision::check_rect(box, box_tile)) {
                                                                    tile_found = true;
                                                                    Game::set_selection(Game_Selection::Type::CITY,
                                                                                        tile.get_parent());
                                                                }
                                                            }
                                                        } else if (tile.get_type() == Tile::Type::BUILDING_UNFINISHED) {
                                                            // If the tile is of type BUILDING_UNFINISHED, its parent is
                                                            // a Civilization
                                                            // If the tile is a member of our civilization
                                                            if (tile.get_parent() == leader.get_civilization()) {
                                                                Collision_Rect<int32_t> box_tile(tile.get_x(x),
                                                                                                 tile.get_x(y),
                                                                                                 tile.get_size(),
                                                                                                 tile.get_size());

                                                                if (Collision::check_rect(box, box_tile)) {
                                                                    tile_found = true;

                                                                    Game::set_selection(
                                                                        Game_Selection::Type::UNFINISHED_BUILDING,
                                                                        tile_coords);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        if (!tile_found) {
                                            Game::clear_selection();
                                        }
                                    }
                                }
                            }
                        }

                        event_consumed = true;
                    } else if (!event_consumed && Engine_Input::event.button.button == SDL_BUTTON_RIGHT) {
                        if (Game::has_order()) {
                            Game::clear_order();
                        }

                        event_consumed = true;
                    }
                }
            }
        }
    }

    return event_consumed;
}

bool Game_Manager::handle_input_events () {
    bool event_consumed = false;

    if (in_progress && Game::started) {
        const vector<Game_Command>& game_commands = Object_Manager::get_game_commands();

        for (size_t i = 0; i < game_commands.size() && !event_consumed; i++) {
            if ((Engine_Input::event.type == SDL_CONTROLLERBUTTONDOWN &&
                 game_commands[i].button == Engine_Input::event.cbutton.button) ||
                (Engine_Input::event.type == SDL_KEYDOWN && Engine_Input::event.key.repeat == 0 &&
                 game_commands[i].key == Engine_Input::event.key.keysym.scancode)) {
                event_consumed = handle_game_command(game_commands[i].name);
            }
        }
    }

    return event_consumed;
}
