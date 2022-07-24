#pragma once

#include "action_in.hpp"
#include "action_out.hpp"
#include "tcp_server.hpp"
#include "players_container.hpp"
#include "rauter.hpp"
#include "subscribs.hpp"
#include "server_socket.hpp"


namespace poker
{

class ServerManager
{
public:
    ServerManager(std::string a_server_ip, int a_servr_port);

    void stop();

private:
    ServerSocket m_socket;
    TcpServer m_tcp;
    ActionOut m_action_out;
    PlayersContainer m_players;
    Subscribs m_subscribs;
    ActionIn m_action_in;
    Rauter m_rauter;
};


}// poker namespace
