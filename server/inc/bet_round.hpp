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
    void chack_in();
    void fold_in();
    void start_bet();
    void finish_bet();

private:
    bool only_one_player_left();
    void bet();
    void next();
    void close_bet_round();
    void zero_bets_and_clear_actions();

private:
    int m_min_bet;
    bool m_stop;
    bool m_wait_for_bet;
    std::mutex m_mutex;
    std::condition_variable m_cond_var;  

    PlayersContainer& m_players;
    ActionOut& m_action_out;
    Table& m_table;
    playerIterator m_open_player;
    playerIterator m_turn;
};


}// poker namespace
