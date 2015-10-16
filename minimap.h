/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef minimap_h
#define minimap_h

#include <image_data.h>

#include <cstdint>

#include <SDL.h>

class Minimap{
private:

    SDL_Surface* surface;

    Image_Data image_data;

    std::uint32_t width;
    std::uint32_t height;

    //pixels
    std::uint32_t chunk_width;
    std::uint32_t chunk_height;

    bool is_generated();

    void clear_surface();
    void clear_image_data();

public:

    Minimap();

    void generate_surface(std::uint32_t new_width,std::uint32_t new_height);
    void update();
    void clear_map();

    void render();
};

#endif
