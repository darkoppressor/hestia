/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef leader_h
#define leader_h

#include <color.h>

#include <cstdint>
#include <string>

class Leader{
private:

    //If player_controlled is false, this Leader is controlled by the AI instead of a player
    bool player_controlled;
    std::uint32_t parent_player;

    std::uint32_t civilization;

    Color color;

public:

    Leader();
    Leader(std::uint32_t new_parent);

    bool is_player_controlled() const;
    std::uint32_t get_parent_player() const;
    void set_parent_player(std::uint32_t new_parent);
    void set_parent_to_ai();

    std::uint32_t get_civilization() const;
    void set_civilization(std::uint32_t new_civilization);

    Color get_color() const;
    static std::string get_color(std::uint32_t index);
    void set_color(const Color& new_color);
};

#endif
