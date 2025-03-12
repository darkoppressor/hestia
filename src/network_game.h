/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef network_game_h
#define network_game_h

#include "game_order.h"

#include <network_message_identifiers.h>

#include <string>

#include "raknet/Source/BitStream.h"

enum {
    ID_GAME_START = ID_GAME_PACKET_ENUM,
    ID_GAME_CLIENT_ORDER
};

enum {
    ORDERING_CHANNEL_GAME_START = ORDERING_CHANNEL_GAME_PACKET_ENUM,
    ORDERING_CHANNEL_GAME_CLIENT_ORDER
};

class Network_Game {
    public:
        static bool receive_game_packet(RakNet::Packet* packet, const RakNet::MessageID& packet_id);

        // Returns an empty string if a new connection should be allowed
        // Otherwise, returns a string containing the reason a new connection is not allowed
        static std::string allow_new_connection();

        static void write_initial_game_data(RakNet::BitStream& bitstream);
        static void read_initial_game_data(RakNet::BitStream& bitstream);

        static void write_update(RakNet::BitStream& bitstream);
        static void read_update(RakNet::BitStream& bitstream);

        // These are used in the lockstep networking model
        static void write_server_ready(RakNet::BitStream& bitstream);
        static void read_server_ready(RakNet::BitStream& bitstream);
        static void write_client_ready(RakNet::BitStream& bitstream);
        static void read_client_ready(RakNet::BitStream& bitstream);

        static void send_game_start_data();
        static void receive_game_start_data();

        static void send_client_game_order(const Game_Order& order);
        static void receive_client_game_order();
};

#endif
