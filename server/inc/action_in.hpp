#pragma once

#include "protocol.hpp"
#include "tcp_server.hpp"
#include "action_out.hpp"
#include "players_container.hpp"
#include "subscribs.hpp"
#include "bet_round.hpp"

namespace poker
{

class ActionIn
{
public:
    ActionIn(TcpServer& m_tcp, ActionOut& a_action_out, PlayersContainer& a_players, Subscribs& a_subscribs, BetRound& a_bet_round);

    void get(char* a_buffer, int a_client_socket);

private:
    void registration_reques(char* a_buffer, int a_client_socket);
    void log_in_reques(char* a_buffer, int a_client_socket);
    void start_bet(char* a_buffer);
    void bet(char* a_buffer);
    void finish_bet(char* a_buffer);
    void check(char* a_buffer);
    void fold(char* a_buffer);
    void wake_up_client(int a_client_socket);

private:
    TcpServer& m_tcp;
    ActionOut& m_action_out; //ned to delete activate modols function
    PlayersContainer& m_players;
    Subscribs& m_subscribs;
    BetRound& m_bet_round;
};


}// poker namespace
