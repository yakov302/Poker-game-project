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
#include "server_socket.hpp"
#include "players_container.hpp"

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
    Table m_table;
    BetRound m_bet_round;
    CardRound m_card_round;
    Game m_game;
    ActionIn m_action_in;
    Rauter m_rauter;
};


}// poker namespace
