/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef person_h
#define person_h

#include "int_math.h"
#include "ai.h"

#include <collision.h>
#include <rng.h>
#include <coords.h>

#include <cstdint>

class Person{
private:

    std::uint32_t parent_city;

    Collision_Rect<std::int32_t> box;

    Int_Vector velocity;
    Int_Vector acceleration;
    Int_Vector force;

    AI_Goal goal;

    ///QQQ Inventory

public:

    Person();
    Person(std::uint32_t new_parent,const Collision_Rect<std::int32_t>& new_box);

    std::uint32_t get_parent_city() const;
    void set_parent_city(std::uint32_t new_parent);

    bool has_goal() const;

    //pixels
    //Returns the range^2
    std::uint64_t get_goal_range() const;
    //pixels
    //Returns coordinates of -1,-1 if the goal coordinates could not be determined
    Coords<std::int32_t> get_goal_coords() const;
    bool goal_within_range() const;
    std::int32_t get_angle_to_goal() const;

    Collision_Rect<std::uint32_t> get_gather_zone() const;
    void find_gather_tile(RNG& rng);

    void ai(RNG& rng);

    void brake();
    void accelerate();
    void movement();

    void render() const;
};

#endif
