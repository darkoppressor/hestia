/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "network_game.h"
#include "../game/game.h"

#include <network/network_engine.h>
#include <game/game_world.h>
#include <color/color.h>
#include <game/game_manager.h>
#include <network/network_server.h>
#include <engine.h>

#include <cstdint>

using namespace std;

bool Network_Game::receive_game_packet (RakNet::Packet* packet, const RakNet::MessageID& packet_id) {
    if (packet_id == ID_GAME_START) {
        if (Network_Engine::status == "client") {
            receive_game_start_data();
        }

        return true;
    } else if (packet_id == ID_GAME_CLIENT_ORDER) {
        if (Network_Engine::status == "server") {
            receive_client_game_order();
        }

        return true;
    }

    return false;
}

string Network_Game::allow_new_connection () {
    if (Game::started) {
        return "Game is already started.";
    }

    return "";
}

void Network_Game::write_initial_game_data (RakNet::BitStream& bitstream) {
    // bitstream.WriteCompressed();
}

void Network_Game::read_initial_game_data (RakNet::BitStream& bitstream) {
    // bitstream.ReadCompressed();
}

void Network_Game::write_update (RakNet::BitStream& bitstream) {
    // bitstream.WriteCompressed();
}

void Network_Game::read_update (RakNet::BitStream& bitstream) {
    // bitstream.ReadCompressed();
}

void Network_Game::write_server_ready (RakNet::BitStream& bitstream) {
    vector<Game_Order> server_game_orders = Game::get_server_game_orders();

    bitstream.WriteCompressed((uint32_t) server_game_orders.size());

    for (size_t i = 0; i < server_game_orders.size(); i++) {
        bitstream.WriteCompressed((uint8_t) server_game_orders[i].type);
        bitstream.WriteCompressed(server_game_orders[i].coords.x);
        bitstream.WriteCompressed(server_game_orders[i].coords.y);
        bitstream.WriteCompressed(server_game_orders[i].leader);

        // Add the order to our own client orders list
        Game::add_client_game_order(server_game_orders[i]);
    }

    Game::clear_server_game_orders();
}

void Network_Game::read_server_ready (RakNet::BitStream& bitstream) {
    uint32_t orders_size = 0;

    bitstream.ReadCompressed(orders_size);

    for (uint32_t i = 0; i < orders_size; i++) {
        uint8_t type = 0;

        bitstream.ReadCompressed(type);

        Coords<uint32_t> coords;

        bitstream.ReadCompressed(coords.x);
        bitstream.ReadCompressed(coords.y);

        uint32_t leader = 0;

        bitstream.ReadCompressed(leader);

        // Add the order to our client orders list
        Game::add_client_game_order(Game_Order((Game_Order::Type) type, coords, leader));
    }
}

void Network_Game::write_client_ready (RakNet::BitStream& bitstream) {
    bitstream.WriteCompressed(Game::get_checksum());
}

void Network_Game::read_client_ready (RakNet::BitStream& bitstream) {
    uint32_t checksum = 0;

    bitstream.ReadCompressed(checksum);

    if (checksum != Game::get_checksum()) {
        if (!Game_Manager::paused) {
            string client_name = "";
            Client_Data* client = Network_Engine::get_packet_client();

            if (client != 0) {
                client_name = client->name;
            }

            string message = "Desync detected between server and client '" + client_name + "'";

            Engine::chat.add_text(message);

            Network_Engine::send_chat_message(message, RakNet::UNASSIGNED_RAKNET_GUID, true);

            Game_Manager::toggle_pause();

            Network_Server::send_paused();
        }
    }
}

void Network_Game::send_game_start_data () {
    if (Network_Engine::status == "server") {
        RakNet::BitStream bitstream;

        bitstream.Write((RakNet::MessageID) ID_GAME_START);

        bitstream.WriteCompressed(Game::option_rng_seed);
        bitstream.WriteCompressed(Game::option_world_width);
        bitstream.WriteCompressed(Game::option_world_height);
        bitstream.WriteCompressed(Game::option_region_min);
        bitstream.WriteCompressed(Game::option_region_max);
        bitstream.WriteCompressed(Game::option_initial_tile_growth);
        bitstream.WriteCompressed(Game::option_vc_conquest);

        bitstream.WriteCompressed(Game::get_leader_count());

        for (uint32_t i = 0; i < Game::get_leader_count(); i++) {
            const Leader& leader = Game::get_leader(i);

            bitstream.WriteCompressed(leader.is_player_controlled());
            bitstream.WriteCompressed(leader.get_parent_player());

            Color color = leader.get_color();

            bitstream.WriteCompressed((int16_t) color.get_red());
            bitstream.WriteCompressed((int16_t) color.get_green());
            bitstream.WriteCompressed((int16_t) color.get_blue());
            bitstream.WriteCompressed((int16_t) color.get_alpha());

            vector<Leader::Diplomacy_State> diplomacy_states = leader.get_diplomacy_states();

            bitstream.WriteCompressed((uint32_t) diplomacy_states.size());

            for (size_t n = 0; n < diplomacy_states.size(); n++) {
                bitstream.WriteCompressed((uint8_t) diplomacy_states[n]);
            }
        }

        Network_Engine::stat_counter_bytes_sent += bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, ORDERING_CHANNEL_GAME_START,
                                   RakNet::UNASSIGNED_RAKNET_GUID, true);
    }
}

void Network_Game::receive_game_start_data () {
    Game_World::clear_world();

    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    bitstream.ReadCompressed(Game::option_rng_seed);
    bitstream.ReadCompressed(Game::option_world_width);
    bitstream.ReadCompressed(Game::option_world_height);
    bitstream.ReadCompressed(Game::option_region_min);
    bitstream.ReadCompressed(Game::option_region_max);
    bitstream.ReadCompressed(Game::option_initial_tile_growth);
    bitstream.ReadCompressed(Game::option_vc_conquest);

    uint32_t leaders_size = 0;

    bitstream.ReadCompressed(leaders_size);

    for (uint32_t i = 0; i < leaders_size; i++) {
        bool player_controlled = false;

        bitstream.ReadCompressed(player_controlled);

        uint32_t parent_player = 0;

        bitstream.ReadCompressed(parent_player);

        int16_t red = 0;
        int16_t green = 0;
        int16_t blue = 0;
        int16_t alpha = 0;

        bitstream.ReadCompressed(red);
        bitstream.ReadCompressed(green);
        bitstream.ReadCompressed(blue);
        bitstream.ReadCompressed(alpha);

        Color color(red, green, blue, alpha);
        uint32_t diplomacy_states_size = 0;

        bitstream.ReadCompressed(diplomacy_states_size);

        vector<Leader::Diplomacy_State> diplomacy_states;

        for (uint32_t n = 0; n < diplomacy_states_size; n++) {
            uint8_t diplomacy_state = 0;

            bitstream.ReadCompressed(diplomacy_state);

            diplomacy_states.push_back((Leader::Diplomacy_State) diplomacy_state);
        }

        Game::add_leader(player_controlled, parent_player, color, diplomacy_states);
    }

    Game_World::generate_world();
}

void Network_Game::send_client_game_order (const Game_Order& order) {
    if (Network_Engine::status == "client") {
        RakNet::BitStream bitstream;

        bitstream.Write((RakNet::MessageID) ID_GAME_CLIENT_ORDER);

        bitstream.WriteCompressed((uint8_t) order.type);
        bitstream.WriteCompressed(order.coords.x);
        bitstream.WriteCompressed(order.coords.y);
        bitstream.WriteCompressed(order.leader);

        Network_Engine::stat_counter_bytes_sent += bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, ORDERING_CHANNEL_GAME_CLIENT_ORDER,
                                   Network_Engine::server_id, false);
    }
}

void Network_Game::receive_client_game_order () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    uint8_t type = 0;

    bitstream.ReadCompressed(type);

    Coords<uint32_t> coords;

    bitstream.ReadCompressed(coords.x);
    bitstream.ReadCompressed(coords.y);

    uint32_t leader = 0;

    bitstream.ReadCompressed(leader);

    Game::add_server_game_order(Game_Order((Game_Order::Type) type, coords, leader));
}
