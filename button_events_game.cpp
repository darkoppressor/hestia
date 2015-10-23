/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "network_game.h"
#include "game.h"

#include <button_events.h>
#include <network_engine.h>
#include <window.h>
#include <window_manager.h>
#include <game_world.h>

using namespace std;

bool Button_Events::handle_button_event_game(string button_event,Window* parent_window,bool& window_opened_on_top){
    if(button_event=="lobby_window"){
        handle_button_event("start_server_lockstep",parent_window);

        if(Network_Engine::status=="server"){
            Window* window=Window_Manager::get_window("lobby");

            window->toggle_on();
            window_opened_on_top=true;
        }

        return true;
    }
    else if(button_event=="game_start_server_lockstep"){
        if(Network_Engine::status=="server"){
            Window_Manager::close_all_windows();

            Game_World::clear_world();

            Game::setup_leaders();

            Network_Game::send_game_start_data();

            Game_World::generate_world();
        }

        return true;
    }
    else if(button_event=="repopulate_city"){
        if(Game::started){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::CITY){
                int32_t leader=Game::get_our_leader();
                if(leader>=0){
                    Game::issue_order(Game_Order(Game_Order::Type::REPOPULATE_CITY,Coords<uint32_t>(selection.index,0),(uint32_t)leader));
                }
            }
        }

        return true;
    }
    else if(button_event=="abandon_city"){
        if(Game::started){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::CITY){
                int32_t leader=Game::get_our_leader();
                if(leader>=0){
                    Game::issue_order(Game_Order(Game_Order::Type::ABANDON_CITY,Coords<uint32_t>(selection.index,0),(uint32_t)leader));
                }
            }
        }

        return true;
    }
    else if(button_event=="cancel_unfinished_building"){
        if(Game::started){
            Game_Selection selection=Game::get_selection();

            if(selection.type==Game_Selection::Type::UNFINISHED_BUILDING){
                int32_t leader=Game::get_our_leader();
                if(leader>=0){
                    Game::issue_order(Game_Order(Game_Order::Type::CANCEL_UNFINISHED_BUILDING,selection.tile_coordinates,(uint32_t)leader));
                }
            }
        }

        return true;
    }

    return false;
}
