/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ai_h
#define ai_h

#include <coords.h>

#include <cstdint>

class AI_Goal{
public:

    enum class Type : std::uint8_t{
        NONE,
        GATHER_WHEAT,
        GATHER_TREE
    };

    Type type;

    Coords<std::uint32_t> coords;

    AI_Goal();
};

#endif
