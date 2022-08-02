#pragma once

#include <condition_variable>

#include "players_container.hpp"
#include "action_out.hpp"
#include "table.hpp"

namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class BetRound
{
public:
    BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table);

    void run(playerIterator a_open_player);
    void bet_in(int a_amount);
    void start_bet();
    void finish_bet();
    void chack_in();
    void fold_in();

private:
    void bet();
    void next();
    void set_max_bet();
    void close_bet_round();
    bool one_player_left();
    void zero_bets_and_clear_actions();

private:
    int m_min_bet;
    int m_max_bet;
    bool m_stop;

    std::mutex m_mutex;
    std::condition_variable m_cond_var;

    Wait m_wait;
    PlayersContainer& m_players;
    ActionOut& m_action_out;
    Table& m_table;

    playerIterator m_turn;
    playerIterator m_open_player;
};


}// poker namespace
