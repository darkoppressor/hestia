/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "tile.h"
#include "game_constants.h"
#include "game.h"

#include <collision/collision.h>
#include <render/render.h>
#include <game/game_manager.h>
#include <image/image_manager.h>
#include <data/object_manager.h>
#include <strings/engine_strings.h>

// TODO includes
// #include <ui/font.h>
//

using namespace std;

Tile::Tile () {
    parent = 0;

    type = Type::WHEAT;

    health = get_health_max();
}

Tile::Tile (Type new_type) {
    parent = 0;

    type = new_type;

    if (type == Type::BUILDING_UNFINISHED) {
        health = 1;
    } else {
        health = get_health_max();
    }
}

Tile::Tile (uint32_t new_parent, Type new_type) {
    parent = new_parent;

    type = new_type;

    if (type == Type::BUILDING_UNFINISHED) {
        health = 1;
    } else {
        health = get_health_max();
    }
}

void Tile::add_checksum_data (vector<uint32_t>& data) const {
    data.push_back(parent);

    data.push_back((uint32_t) type);

    data.push_back((uint32_t) health);
}

uint32_t Tile::get_parent () const {
    return parent;
}

void Tile::set_parent (uint32_t new_parent) {
    parent = new_parent;
}

Tile::Type Tile::get_type () const {
    return type;
}

void Tile::set_type (Type new_type) {
    type = new_type;

    health = get_health_max();
}

bool Tile::is_gatherable () const {
    return type == Type::WHEAT || type == Type::TREE;
}

bool Tile::is_food () const {
    return type == Type::WHEAT;
}

bool Tile::is_alive () const {
    return health > 0;
}

bool Tile::health_low () const {
    return health <= get_health_max() / 4;
}

bool Tile::needs_repair () const {
    return health < get_health_max();
}

uint32_t Tile::repair_count_needed () const {
    if (needs_repair()) {
        int16_t health_needed = get_health_max() - health;
        int16_t denominator = 100 / Game_Constants::BUILDING_REPAIR_AMOUNT;
        int16_t repair_amount = get_health_max() / denominator;
        int16_t repairs_needed = health_needed / repair_amount;

        if (health_needed % repair_amount != 0) {
            repairs_needed++;
        }

        return (uint32_t) repairs_needed;
    } else {
        return 0;
    }
}

void Tile::repair () {
    int16_t denominator = 100 / Game_Constants::BUILDING_REPAIR_AMOUNT;

    health += get_health_max() / denominator;

    if (health > get_health_max()) {
        health = get_health_max();
    }
}

void Tile::capture_heal () {
    int16_t denominator = 100 / Game_Constants::BUILDING_CAPTURED_HEALTH_PERCENTAGE;

    if (denominator == 0) {
        denominator = 1;
    }

    health = get_health_max() / denominator;

    if (health == 0) {
        health = 1;
    }
}

void Tile::damage (int16_t attack) {
    int16_t damage_done = attack - get_defense();

    if (damage_done < 0) {
        damage_done = 0;
    }

    health -= damage_done;
}

void Tile::die () {
    health = 0;
}

int16_t Tile::get_health_max () const {
    return Game_Constants::BUILDING_HEALTH_MAX;
}

int16_t Tile::get_health () const {
    return health;
}

int16_t Tile::get_defense () const {
    return Game_Constants::BUILDING_DEFENSE;
}

bool Tile::tile_type_is_building (Type type_to_check) {
    if (type_to_check == Type::BUILDING_CITY || type_to_check == Type::BUILDING_UNFINISHED) {
        return true;
    } else {
        return false;
    }
}

bool Tile::is_building () const {
    if (tile_type_is_building(type)) {
        return true;
    } else {
        return false;
    }
}

int32_t Tile::get_tile_type_size (Type type_to_check) {
    if (tile_type_is_building(type_to_check)) {
        return int32_t(Game_Constants::BUILDING_SIZE * Game_Constants::TILE_SIZE);
    } else {
        return (int32_t) Game_Constants::TILE_SIZE;
    }
}

int32_t Tile::get_size () const {
    return get_tile_type_size(type);
}

int32_t Tile::get_x (uint32_t tile_x) {
    return int32_t(tile_x * Game_Constants::TILE_SIZE);
}

int32_t Tile::get_y (uint32_t tile_y) {
    return int32_t(tile_y * Game_Constants::TILE_SIZE);
}

int32_t Tile::get_center_x (uint32_t tile_x, int32_t tile_size) {
    return int32_t(tile_x * Game_Constants::TILE_SIZE) + tile_size / 2;
}

int32_t Tile::get_center_y (uint32_t tile_y, int32_t tile_size) {
    return int32_t(tile_y * Game_Constants::TILE_SIZE) + tile_size / 2;
}

int32_t Tile::get_center_x (uint32_t tile_x) const {
    return get_center_x(tile_x, get_size());
}

int32_t Tile::get_center_y (uint32_t tile_y) const {
    return get_center_y(tile_y, get_size());
}

uint32_t Tile::get_chunk_x (uint32_t tile_x) {
    return tile_x / Game_Constants::CHUNK_SIZE;
}

uint32_t Tile::get_chunk_y (uint32_t tile_y) {
    return tile_y / Game_Constants::CHUNK_SIZE;
}

string Tile::get_type_string (Type type_to_check) {
    if (type_to_check == Type::WHEAT) {
        return "wheat";
    } else if (type_to_check == Type::TREE) {
        return "tree";
    } else if (type_to_check == Type::BUILDING_UNFINISHED) {
        return "building_unfinished";
    } else if (type_to_check == Type::BUILDING_CITY) {
        return "building_city";
    } else {
        return "";
    }
}

Color* Tile::get_type_color (Type type_to_check) {
    string type_string = get_type_string(type_to_check);

    if (type_string.length() > 0) {
        return Object_Manager::get_color("tile_" + type_string);
    } else {
        return Object_Manager::get_color("tile_wheat");
    }
}

void Tile::write_info_string (string& text) const {
    if (type == Type::BUILDING_UNFINISHED) {
        text += "Health: " + Strings::num_to_string(get_health()) + "/" + Strings::num_to_string(get_health_max()) +
                "\n";
    }
}

void Tile::render (uint32_t tile_x, uint32_t tile_y, bool selected) const {
    if (is_alive()) {
        // pixels
        double x = get_x(tile_x);
        double y = get_y(tile_y);
        Collision_Rect<double> box_render(x, y, (double) get_size(), (double) get_size());

        if (Collision::check_rect(box_render * Game_Manager::camera_zoom, Game_Manager::camera)) {
            if (type == Type::BUILDING_UNFINISHED) {
                const Civilization& civilization = Game::get_civilization(get_parent());
                string color = civilization.get_color();

                Render::render_rectangle((x - Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR) *
                                         Game_Manager::camera_zoom - Game_Manager::camera.x,
                                         (y - Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR) *
                                         Game_Manager::camera_zoom - Game_Manager::camera.y,
                                         ((double) get_size() + Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR *
                                          2.0) * Game_Manager::camera_zoom,
                                         ((double) get_size() + Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR *
                                          2.0) * Game_Manager::camera_zoom, 0.5, color);

                Render::render_rectangle_empty((x - Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR) *
                                               Game_Manager::camera_zoom - Game_Manager::camera.x,
                                               (y - Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR) *
                                               Game_Manager::camera_zoom - Game_Manager::camera.y,
                                               ((double) get_size() + Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR *
                                                2.0) * Game_Manager::camera_zoom,
                                               ((double) get_size() + Game_Constants::RENDER_BUILDING_UNFINISHED_COLOR *
                                                2.0) * Game_Manager::camera_zoom, 1.0, color,
                                               Game_Constants::RENDER_BUILDING_COLOR_BORDER *
                                               Game_Manager::camera_zoom);
            }

            Render::render_texture(x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                   y * Game_Manager::camera_zoom - Game_Manager::camera.y,
                                   Image_Manager::get_image("tile_" + get_type_string(type)), 1.0,
                                   Game_Manager::camera_zoom, Game_Manager::camera_zoom);

            if (is_building()) {
                double percentage = (double) get_health() / (double) get_health_max();
                double max_bar_width = (double) get_size();
                double bar_width = max_bar_width * percentage;
                double bar_height = Game_Constants::RENDER_BAR_HEIGHT;
                double back_thickness = Game_Constants::RENDER_BAR_THICKNESS;
                string color_bar_health = "health_normal";

                if (health_low()) {
                    color_bar_health = "health_low";
                }

                Render::render_rectangle((x - back_thickness) * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                         (y - bar_height - back_thickness * 4.0) * Game_Manager::camera_zoom -
                                         Game_Manager::camera.y,
                                         (max_bar_width + back_thickness * 2.0) * Game_Manager::camera_zoom,
                                         (bar_height + back_thickness * 2.0) * Game_Manager::camera_zoom, 1.0,
                                         "ui_black");
                Render::render_rectangle(x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                         (y - bar_height - back_thickness * 3.0) * Game_Manager::camera_zoom -
                                         Game_Manager::camera.y, bar_width * Game_Manager::camera_zoom,
                                         bar_height * Game_Manager::camera_zoom, 1.0, color_bar_health);
            }

            if (selected) {
                Render::render_rectangle_empty((x - Game_Constants::RENDER_SELECTION_SIZE) * Game_Manager::camera_zoom -
                                               Game_Manager::camera.x,
                                               (y - Game_Constants::RENDER_SELECTION_SIZE) * Game_Manager::camera_zoom -
                                               Game_Manager::camera.y,
                                               ((double) get_size() + Game_Constants::RENDER_SELECTION_SIZE * 2.0) *
                                               Game_Manager::camera_zoom,
                                               ((double) get_size() + Game_Constants::RENDER_SELECTION_SIZE * 2.0) *
                                               Game_Manager::camera_zoom, 1.0, "selection",
                                               Game_Constants::RENDER_SELECTION_BORDER * Game_Manager::camera_zoom);
            }

            // TODO dev data
            // Bitmap_Font* font=Object_Manager::get_font("small");
            // font->show(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,Strings::num_to_string(tile_x)+","+Strings::num_to_string(tile_y),"red");
            //
        }
    }
}
