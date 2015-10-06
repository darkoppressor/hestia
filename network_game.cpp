/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "network_game.h"
#include "game.h"

#include <network_lockstep.h>
#include <network_engine.h>
#include <game_world.h>
#include <color.h>

#include <cstdint>

using namespace std;

bool Network_Game::receive_game_packet(RakNet::Packet* packet,const RakNet::MessageID& packet_id){
    if(packet_id==ID_GAME_START){
        if(Network_Engine::status=="client"){
            receive_game_start_data();
        }

        return true;
    }

    return false;
}

string Network_Game::allow_new_connection(){
    if(Game::started){
        return "Game is already started.";
    }

    return "";
}

void Network_Game::write_initial_game_data(RakNet::BitStream* bitstream){
    ///bitstream->WriteCompressed();
}

void Network_Game::read_initial_game_data(RakNet::BitStream* bitstream){
    ///bitstream->ReadCompressed();
}

void Network_Game::write_update(RakNet::BitStream* bitstream){
    ///bitstream->WriteCompressed();
}

void Network_Game::read_update(RakNet::BitStream* bitstream){
    ///bitstream->ReadCompressed();
}

void Network_Game::write_server_ready(RakNet::BitStream* bitstream){
    bitstream->WriteCompressed(Network_Lockstep::get_server_completed_turn());

    ///Write game commands
}

void Network_Game::read_server_ready(RakNet::BitStream* bitstream){
    uint32_t server_completed_turn=0;
    bitstream->ReadCompressed(server_completed_turn);
    Network_Lockstep::set_server_completed_turn(server_completed_turn);

    ///Read game commands
}

void Network_Game::send_game_start_data(){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_START);

        bitstream.WriteCompressed(Game::option_rng_seed);
        bitstream.WriteCompressed(Game::option_world_width);
        bitstream.WriteCompressed(Game::option_world_height);
        bitstream.WriteCompressed(Game::option_region_min);
        bitstream.WriteCompressed(Game::option_region_max);

        bitstream.WriteCompressed((uint32_t)Game::leaders.size());
        for(size_t i=0;i<Game::leaders.size();i++){
            bitstream.WriteCompressed(Game::leaders[i].is_player_controlled());
            bitstream.WriteCompressed(Game::leaders[i].get_parent_player());

            Color color=Game::leaders[i].get_color();
            bitstream.WriteCompressed((int16_t)color.get_red());
            bitstream.WriteCompressed((int16_t)color.get_green());
            bitstream.WriteCompressed((int16_t)color.get_blue());
            bitstream.WriteCompressed((int16_t)color.get_alpha());
        }

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_GAME_START,RakNet::UNASSIGNED_RAKNET_GUID,true);
    }
}

void Network_Game::receive_game_start_data(){
    RakNet::BitStream bitstream(Network_Engine::packet->data,Network_Engine::packet->length,false);
    Network_Engine::stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    bitstream.ReadCompressed(Game::option_rng_seed);
    bitstream.ReadCompressed(Game::option_world_width);
    bitstream.ReadCompressed(Game::option_world_height);
    bitstream.ReadCompressed(Game::option_region_min);
    bitstream.ReadCompressed(Game::option_region_max);

    uint32_t leaders_size=0;
    bitstream.ReadCompressed(leaders_size);
    for(uint32_t i=0;i<leaders_size;i++){
        bool player_controlled=false;
        bitstream.ReadCompressed(player_controlled);

        uint32_t parent_player=0;
        bitstream.ReadCompressed(parent_player);

        if(player_controlled){
            Game::leaders.push_back(Leader(parent_player));
        }
        else{
            Game::leaders.push_back(Leader());
        }

        int16_t red=0;
        int16_t green=0;
        int16_t blue=0;
        int16_t alpha=0;
        bitstream.ReadCompressed(red);
        bitstream.ReadCompressed(green);
        bitstream.ReadCompressed(blue);
        bitstream.ReadCompressed(alpha);

        Color color(red,green,blue,alpha);
        Game::leaders.back().set_color(color);
    }

    Game_World::generate_world();
}
