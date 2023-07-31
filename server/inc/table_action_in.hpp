#pragma once

#include "protocol.hpp"
#include "bet_round.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

namespace poker
{

class TableActionIn
{
public:
    TableActionIn(ActionOut& a_action_out, PlayersContainer& a_players, BetRound& a_bet_round);

    void get(char* a_buffer, int a_client_socket);
    void player_deleted(int a_client_socket);

private:
    void finish_bet();
    void start_bet();
    void bet(char* a_buffer);

    void fold();
    void check();

private:
    BetRound& m_bet_round;
    ActionOut& m_action_out; 
    PlayersContainer& m_players;
};


}// poker namespace
