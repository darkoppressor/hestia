/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef leader_h
#define leader_h

#include <color.h>

#include <cstdint>
#include <string>
#include <vector>

class Leader {
    public:
        enum class Diplomacy_State : std::uint8_t {
            IS_FRIENDS_WITH,
            IS_ENEMIES_WITH,
            IS_NEUTRAL_TOWARDS
        };

    private:
        // If player_controlled is false, this Leader is controlled by the AI instead of a player
        bool player_controlled;
        std::uint32_t parent_player;

        std::uint32_t civilization;
        Color color;

        // One element will be generated per leader
        // Each elements represents the diplomacy state with the leader with the corresponding index
        std::vector<Diplomacy_State> diplomacy_states;

    public:
        Leader ();
        Leader (std::uint32_t new_parent);

        void add_checksum_data(std::vector<std::uint32_t>& data) const;

        bool is_player_controlled() const;
        std::uint32_t get_parent_player() const;
        void set_parent_player(std::uint32_t new_parent);
        void set_parent_to_ai();

        std::uint32_t get_civilization() const;
        void set_civilization(std::uint32_t new_civilization);

        Color get_color() const;
        static std::string get_color(std::uint32_t index);
        void set_color(const Color& new_color);

        std::vector<Diplomacy_State> get_diplomacy_states() const;
        void set_diplomacy_states(const std::vector<Diplomacy_State>& new_diplomacy_states);
        // Create an initial diplomacy states list for the leader, to be used by the host
        void initialize_diplomacy_states(std::uint32_t index, std::uint32_t leader_count);

        bool is_friends_with(std::uint32_t leader_index) const;
        bool is_enemies_with(std::uint32_t leader_index) const;
        bool is_neutral_towards(std::uint32_t leader_index) const;

        bool is_defeated() const;
};

#endif
