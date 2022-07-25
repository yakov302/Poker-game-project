#pragma once

#include "players_container.hpp"
#include "action_out.hpp"
#include "table.hpp"

namespace poker
{

class BetRound
{
public:
    BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table);

    void run(playerIterator a_open_player);
    void bet_in(int a_amount);
    void finish_bet();

private:
    void bet();
    void next();
    void close_bet_round();

private:
    int m_min_bet;
    int m_current_bet;
    bool m_stop;
    bool m_wait_for_bet;
    PlayersContainer& m_players;
    ActionOut& m_action_out;
    Table& m_table;
    playerIterator m_open_player;
    playerIterator m_turn;
};


}// poker namespace
