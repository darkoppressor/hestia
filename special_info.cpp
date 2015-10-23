/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <special_info.h>
#include <log.h>
#include <object_manager.h>

using namespace std;

string Special_Info::get_special_info_text(string special_info){
    string text="";

    if(special_info.length()>0){
        if(special_info=="configure_command"){
            Object_Manager::output_command_configuration_info(text);
        }
        else if(special_info=="person"){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::PERSON){
                const Person& person=Game::get_person(selection.index);

                person.write_info_string(text);
            }
        }
        else if(special_info=="city"){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::CITY){
                const City& city=Game::get_city(selection.index);

                city.write_info_string(text);
            }
        }
        else if(special_info=="unfinished_building"){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::UNFINISHED_BUILDING){
                if(Game::tile_exists(selection.tile_coordinates)){
                    const Tile& tile=Game::get_tile(selection.tile_coordinates);

                    tile.write_info_string(text);
                }
            }
        }
        else if(special_info=="civilization"){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::CIVILIZATION){
                const Civilization& civilization=Game::get_civilization(selection.index);

                civilization.write_info_string(text);
            }
        }
        else{
            Log::add_error("Invalid special info text: '"+special_info+"'");
        }
    }

    return text;
}

string Special_Info::get_special_info_sprite(string special_info){
    string str_sprite_name="";

    if(special_info.length()>0){
        if(special_info=="example"){
            str_sprite_name="";
        }
        else{
            Log::add_error("Invalid special info sprite: '"+special_info+"'");
        }
    }

    return str_sprite_name;
}
