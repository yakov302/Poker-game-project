#pragma once

#include "game.hpp"
#include "table.hpp"
#include "rauter.hpp"
#include "action_in.hpp"
#include "subscribs.hpp"
#include "bet_round.hpp"
#include "card_round.hpp"
#include "action_out.hpp"
#include "tcp_server.hpp"
#include "msg_memory.hpp"
#include "server_socket.hpp"
#include "tables_container.hpp"
#include "players_container.hpp"

#define NUM_OF_MESSAGES_IN_MEMORY 512

namespace poker
{

class ServerManager
{
public:
    ServerManager();

    void stop();

private:
    ServerSocket m_socket;
    TcpServer m_tcp;
    TablesContainer m_tables_container;
    Subscribs m_subscribs;
    MsgMemory m_msg_memory;  
    ActionIn m_action_in;
    Rauter m_rauter;
};


}// poker namespace
