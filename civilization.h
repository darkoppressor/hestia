/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef civilization_h
#define civilization_h

#include <cstdint>
#include <vector>

class Civilization{
private:

    std::uint32_t parent_leader;

    std::vector<std::uint32_t> cities;

public:

    Civilization();
    Civilization(std::uint32_t new_parent_leader);

    std::uint32_t get_parent_leader();
    void set_parent_leader(std::uint32_t new_parent_leader);
};

#endif
