#pragma once

#include "protocol.hpp"
#include "subscribs.hpp"
#include "bet_round.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

namespace poker
{

class ActionIn
{
public:
    ActionIn(ActionOut& a_action_out, PlayersContainer& a_players, Subscribs& a_subscribs, BetRound& a_bet_round);

    void get(char* a_buffer, int a_client_socket);

private:
    void registration_reques(char* a_buffer, int a_client_socket);
    void log_in_reques(char* a_buffer, int a_client_socket);

    void finish_bet();
    void start_bet();
    void bet(char* a_buffer);

    void fold();
    void check();
    void wake_up_client(int a_client_socket);

private:
    BetRound& m_bet_round;
    Subscribs& m_subscribs;
    ActionOut& m_action_out; 
    PlayersContainer& m_players;
};


}// poker namespace
