/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_options_h
#define game_options_h

#include <string>
#include <cstdint>

class Game_Options{
public:

    static double camera_speed_mod_fast;
    static double camera_speed_mod_slow;

    static bool edge_scrolling;
    static double edge_scroll_speed;
    static double edge_scroll_speed_mod_fast;
    static double edge_scroll_speed_mod_slow;

    static bool zoom_to_cursor;

    static bool minimap_show_resources;

    static bool get_option(std::string name,std::string& value);
    static void set_option(std::string name,std::string value);
};

#endif
