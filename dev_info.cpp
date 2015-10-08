/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <engine.h>
#include <game_manager.h>
#include <font.h>
#include <object_manager.h>
#include <options.h>
#include <network_engine.h>
#include <render.h>
#include <engine_strings.h>

using namespace std;

void Engine::render_dev_info(){
    string msg="";

    if(Game_Manager::in_progress){
        msg+="Camera Position: "+Strings::num_to_string(Game_Manager::camera.x)+","+Strings::num_to_string(Game_Manager::camera.y)+"\n";
        msg+="Camera Size: "+Strings::num_to_string(Game_Manager::camera.w/Game_Manager::camera_zoom)+","+Strings::num_to_string(Game_Manager::camera.h/Game_Manager::camera_zoom)+"\n";
        msg+="Camera Zoom: "+Strings::num_to_string(Game_Manager::camera_zoom)+"\n";

        if(Game::started){
            msg+="\n";

            msg+="Day: "+Strings::num_to_string(Game::calendar.get_day())+"\n";
            msg+="Week: "+Strings::num_to_string(Game::calendar.get_week())+"\n";
            msg+="Month: "+Strings::num_to_string(Game::calendar.get_month())+"\n";
            msg+="Year: "+Strings::num_to_string(Game::calendar.get_year())+"\n";

            int player_number=Network_Engine::get_our_player_number();
            if(player_number>=0){
                int32_t leader_index=Game::get_player_leader(player_number);

                if(leader_index>=0){
                    const Leader& leader=Game::get_leader((uint32_t)leader_index);
                    const Civilization& civilization=Game::get_civilization(leader.get_civilization());

                    msg+="\nCivilization:\n";
                    msg+="Wheat: "+Strings::num_to_string(civilization.get_item_count(Inventory::Item_Type::WHEAT))+"\n";
                    msg+="Tree: "+Strings::num_to_string(civilization.get_item_count(Inventory::Item_Type::TREE))+"\n";
                }
            }
        }
    }

    if(msg.length()>0){
        Bitmap_Font* font=Object_Manager::get_font("small");

		double y=2.0;
        if(Options::fps){
            y+=font->spacing_y;

            if(Network_Engine::status!="off"){
                y+=font->spacing_y*2.0;
            }
        }

        Render::render_rectangle(2.0,y,Strings::longest_line(msg)*font->spacing_x,Strings::newline_count(msg)*font->spacing_y,0.75,"ui_black");
        font->show(2.0,y,msg,"red");
    }
}
