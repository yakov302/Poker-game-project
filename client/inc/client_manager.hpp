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
