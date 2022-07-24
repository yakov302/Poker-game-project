#pragma once

#include "table.hpp"
#include "players_container.hpp"
#include "self.hpp"
#include "tcp_client.hpp"
#include "router.hpp"
#include "action_in.hpp"
#include "action_out.hpp"
#include "login.hpp"

namespace poker
{

using playerPointer = std::shared_ptr<Player>;

class ClientManager
{
public:
    ClientManager(std::string a_server_ip, int a_server_port);

private:
    Hand m_cards; 
    Wallet m_chips;
    Self m_self;
    PlayersContainer m_players;
    TcpClient m_tcp;
    ActionOut m_action_out;
    Table m_table;
    ActionIn m_action_in;
    Router m_router; 
};


}// poker namespace
