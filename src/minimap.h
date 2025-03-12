/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef minimap_h
#define minimap_h

#include <image_data.h>
#include <collision.h>

#include <cstdint>

#include <SDL.h>

class Minimap {
    private:
        SDL_Surface* surface;
        Image_Data image_data;

        std::uint32_t width;
        std::uint32_t height;

        // pixels
        std::uint32_t chunk_width;
        std::uint32_t chunk_height;

        bool is_generated();

        void clear_surface();
        void clear_image_data();

    public:
        Minimap ();

        void generate_surface(std::uint32_t new_width, std::uint32_t new_height);
        void update();
        void clear_map();

        Collision_Rect<double> get_box();

        void render();
};

#endif
