/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef game_options_h
#define game_options_h

#include <string>

class Game_Options {
    public:
        static double camera_speed_mod_fast;
        static double camera_speed_mod_slow;

        static bool edge_scrolling;
        static double edge_scroll_speed;
        static double edge_scroll_speed_mod_fast;
        static double edge_scroll_speed_mod_slow;

        static bool zoom_to_cursor;

        static bool minimap_show_resources;

        static bool get_option(std::string name, std::string& value);
        static void set_option(std::string name, std::string value);
};

#endif
