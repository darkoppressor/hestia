/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "game_options.h"

#include <strings/engine_strings.h>

using namespace std;

double Game_Options::camera_speed_mod_fast = 0.0;
double Game_Options::camera_speed_mod_slow = 0.0;
bool Game_Options::edge_scrolling = false;
double Game_Options::edge_scroll_speed = 0.0;
double Game_Options::edge_scroll_speed_mod_fast = 0.0;
double Game_Options::edge_scroll_speed_mod_slow = 0.0;
bool Game_Options::zoom_to_cursor = false;
bool Game_Options::minimap_show_resources = false;

bool Game_Options::get_option (string name, string& value) {
    if (name == "cl_camera_speed_mod_fast") {
        value = Strings::num_to_string(camera_speed_mod_fast);

        return true;
    } else if (name == "cl_camera_speed_mod_slow") {
        value = Strings::num_to_string(camera_speed_mod_slow);

        return true;
    } else if (name == "cl_edge_scrolling") {
        value = Strings::bool_to_string(edge_scrolling);

        return true;
    } else if (name == "cl_edge_scroll_speed") {
        value = Strings::num_to_string(edge_scroll_speed);

        return true;
    } else if (name == "cl_edge_scroll_speed_mod_fast") {
        value = Strings::num_to_string(edge_scroll_speed_mod_fast);

        return true;
    } else if (name == "cl_edge_scroll_speed_mod_slow") {
        value = Strings::num_to_string(edge_scroll_speed_mod_slow);

        return true;
    } else if (name == "cl_zoom_to_cursor") {
        value = Strings::bool_to_string(zoom_to_cursor);

        return true;
    } else if (name == "cl_minimap_show_resources") {
        value = Strings::bool_to_string(minimap_show_resources);

        return true;
    }

    return false;
}

void Game_Options::set_option (string name, string value) {
    if (name == "cl_camera_speed_mod_fast") {
        camera_speed_mod_fast = Strings::string_to_double(value);
    } else if (name == "cl_camera_speed_mod_slow") {
        camera_speed_mod_slow = Strings::string_to_double(value);
    } else if (name == "cl_edge_scrolling") {
        edge_scrolling = Strings::string_to_bool(value);
    } else if (name == "cl_edge_scroll_speed") {
        edge_scroll_speed = Strings::string_to_double(value);
    } else if (name == "cl_edge_scroll_speed_mod_fast") {
        edge_scroll_speed_mod_fast = Strings::string_to_double(value);
    } else if (name == "cl_edge_scroll_speed_mod_slow") {
        edge_scroll_speed_mod_slow = Strings::string_to_double(value);
    } else if (name == "cl_zoom_to_cursor") {
        zoom_to_cursor = Strings::string_to_bool(value);
    } else if (name == "cl_minimap_show_resources") {
        minimap_show_resources = Strings::string_to_bool(value);
    }
}
