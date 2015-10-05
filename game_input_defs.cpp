/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <game_manager.h>
#include <network_engine.h>
#include <network_server.h>
#include <engine.h>
#include <object_manager.h>
#include <options.h>
#include <engine_input.h>

using namespace std;

void Game_Manager::handle_drag_and_drop(string file){
    ///Do something with file
}

void Game_Manager::handle_command_states_multiplayer(){
    if(in_progress && Game::started){
        if(Network_Engine::status=="server"){
            Network_Server::prepare_server_input_states();

            for(size_t i=0;i<Network_Engine::clients.size();i++){
                if(!paused){
                    //Example multiplayer command state
                    /**if(Network_Engine::clients[i].game_command_state("some_command")){
                        ///Deal with command state here
                    }*/
                }
            }
        }
    }
}

void Game_Manager::handle_game_commands_multiplayer(){
    if(in_progress && Game::started){
        if(Network_Engine::status=="server"){
            for(size_t i=0;i<Network_Engine::clients.size();i++){
                for(size_t j=0;j<Network_Engine::clients[i].command_buffer.size();j++){
                    string command_name=Network_Engine::clients[i].command_buffer[j];

                    if(!paused){
                        //Example multiplayer command
                        /**if(command_name=="some_command"){
                            ///Deal with command here
                        }*/
                    }
                }

                Network_Engine::clients[i].command_buffer.clear();
            }
        }
    }
}

void Game_Manager::handle_input_states_gui(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    if(in_progress && Game::started){
        if(Object_Manager::game_command_state("scoreboard")){
            display_scoreboard=true;
        }

        //Move the camera
        if(Game::move_input_state("left")){
            cam_state="left";
        }
        if(Game::move_input_state("up")){
            cam_state="up";
        }
        if(Game::move_input_state("right")){
            cam_state="right";
        }
        if(Game::move_input_state("down")){
            cam_state="down";
        }
        if(Game::move_input_state("left") && Game::move_input_state("up")){
            cam_state="left_up";
        }
        if(Game::move_input_state("up") && Game::move_input_state("right")){
            cam_state="right_up";
        }
        if(Game::move_input_state("right") && Game::move_input_state("down")){
            cam_state="right_down";
        }
        if(Game::move_input_state("down") && Game::move_input_state("left")){
            cam_state="left_down";
        }
        if(Game::move_input_state("left") && Game::move_input_state("right")){
            cam_state="left";
        }
        if(Game::move_input_state("up") && Game::move_input_state("down")){
            cam_state="up";
        }
        if(Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("right")){
            cam_state="left_up";
        }
        if(Game::move_input_state("left") && Game::move_input_state("down") && Game::move_input_state("right")){
            cam_state="left_down";
        }
        if(Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("down")){
            cam_state="left_up";
        }
        if(Game::move_input_state("up") && Game::move_input_state("right") && Game::move_input_state("down")){
            cam_state="right_up";
        }
        if(Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("right") && Game::move_input_state("down")){
            cam_state="left_up";
        }
        if(!Game::move_input_state("left") && !Game::move_input_state("up") && !Game::move_input_state("right") && !Game::move_input_state("down")){
            cam_state="none";
        }
    }
}

void Game_Manager::handle_input_states(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    if(in_progress && Game::started){
        if(!paused){
            //Example multiplayer command state
            /**if(Object_Manager::game_command_state("some_command")){
                command_states.push_back("some_command");
            }*/
        }
    }
}

bool Game_Manager::handle_game_command_gui(string command_name){
    if(Game::started){
        //Multiplayer pause
        if(command_name=="pause"){
            if(Network_Engine::status=="server"){
                toggle_pause();

                Network_Server::send_paused();
            }

            return true;
        }
    }

    //Toggle chat box
    if(command_name=="chat"){
        Engine::chat.toggle_on();

        return true;
    }

    return false;
}

bool Game_Manager::handle_game_command(string command_name){
    const uint8_t* keystates=SDL_GetKeyboardState(NULL);

    ///DEV COMMANDS
    if(Options::dev && keystates[SDL_SCANCODE_F1]){
        //Example dev command
        /**if(command_name=="some_dev_command"){
            ///Dev command here.

            return true;
        }*/
    }
    ///END OF DEV COMMANDS

    if(!paused){
        //Example command
        /**if(command_name=="some_command"){
            ///Command here

            return true;
        }*/

        //Example multiplayer command input
        /**if(command_name=="some_command"){
            Network_Engine::add_command(command_name);

            return true;
        }*/
    }

    return false;
}

bool Game_Manager::handle_input_events_gui(){
    bool event_consumed=false;

    if(in_progress){
        const vector<Game_Command>& game_commands=Object_Manager::get_game_commands();

        for(size_t i=0;i<game_commands.size() && !event_consumed;i++){
            if((Engine_Input::event.type==SDL_CONTROLLERBUTTONDOWN && game_commands[i].button==Engine_Input::event.cbutton.button) ||
               (Engine_Input::event.type==SDL_KEYDOWN && Engine_Input::event.key.repeat==0 && game_commands[i].key==Engine_Input::event.key.keysym.scancode)){
                event_consumed=handle_game_command_gui(game_commands[i].name);
            }
        }

        if(Engine_Input::event.type==SDL_MOUSEWHEEL){
            if(!event_consumed && Engine_Input::event.wheel.y>0){
                zoom_camera_in(camera);

                event_consumed=true;
            }
            else if(!event_consumed && Engine_Input::event.wheel.y<0){
                zoom_camera_out(camera);

                event_consumed=true;
            }
        }
    }

    return event_consumed;
}

bool Game_Manager::handle_input_events(){
    bool event_consumed=false;

    if(in_progress && Game::started){
        const vector<Game_Command>& game_commands=Object_Manager::get_game_commands();

        for(size_t i=0;i<game_commands.size() && !event_consumed;i++){
            if((Engine_Input::event.type==SDL_CONTROLLERBUTTONDOWN && game_commands[i].button==Engine_Input::event.cbutton.button) ||
               (Engine_Input::event.type==SDL_KEYDOWN && Engine_Input::event.key.repeat==0 && game_commands[i].key==Engine_Input::event.key.keysym.scancode)){
                event_consumed=handle_game_command(game_commands[i].name);
            }
        }
    }

    return event_consumed;
}
