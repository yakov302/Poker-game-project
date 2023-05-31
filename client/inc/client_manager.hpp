#pragma once

#include "self.hpp"
#include "table.hpp"
#include "router.hpp"
#include "action_in.hpp"
#include "action_out.hpp"
#include "tcp_client.hpp"
#include "client_socket.hpp"
#include "players_container.hpp"

namespace poker
{

#define HEND_GAP_BETWEEN_FRONT_CARDS 150 
#define HEND_GAP_BETWEEN_BACK_CARDS 0
#define HEND_NUM_OF_CARD_FRONT 5
#define HEND_NUM_OF_CARD_BACK 0
#define HEND_IMAGE_SCALE 1.8
#define HEND_X_POS 850
#define HEND_Y_POS 350

#define WALLET_X_POS 700
#define WALLET_Y_POS 473

class ClientManager
{
public:
    ClientManager();

private:
    Hand m_cards; 
    Wallet m_chips;
    Self m_self;
    PlayersContainer m_players;
    ClientSocket m_socket;
    TcpClient m_tcp;
    ActionOut m_action_out;
    Table m_table;
    ActionIn m_action_in;
    Router m_router; 
};


}// poker namespace
