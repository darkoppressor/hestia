/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef person_h
#define person_h

#include "int_math.h"

#include <collision.h>

#include <cstdint>

class Person{
private:

    std::uint32_t parent_city;

    Collision_Rect<std::int32_t> box;

    Int_Vector velocity;
    Int_Vector acceleration;
    Int_Vector force;

    ///QQQ Inventory

public:

    Person();
    Person(std::uint32_t new_parent,const Collision_Rect<std::int32_t>& new_box);

    std::uint32_t get_parent_city() const;
    void set_parent_city(std::uint32_t new_parent);

    void ai();

    void accelerate();
    void movement();

    void render() const;
};

#endif
