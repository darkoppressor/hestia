/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef leader_h
#define leader_h

#include <color.h>

#include <cstdint>

class Leader{
private:

    //If -1, this Leader is controlled by the AI instead of a player
    std::int32_t parent_player;

    std::uint32_t civilization;

    Color color;

public:

    Leader();
    Leader(std::int32_t new_parent_player);

    std::int32_t get_parent_player();
    void set_parent_player(std::int32_t new_parent_player);

    std::uint32_t get_civilization();
    void set_civilization(std::uint32_t new_civilization);

    Color get_color();
    void set_color(const Color& new_color);
};

#endif
