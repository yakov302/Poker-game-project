#pragma once

#include "players_container.hpp"
#include "action_out.hpp"
#include "table.hpp"
#include "deck.hpp"

#define HUGE_NUMBER 999999999

namespace poker
{

class BetRound
{
public:
    BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table, Deck& a_deck);

    void run(playerIterator a_open_player);
    
    void player_deleted(int a_client_socket);
    void bet_in(int a_amount);
    void finish_bet();
    void start_bet();
    void chack_in();
    void fold_in();

private:
    void next();
    void wait_for_bet();
    bool set_max_bet();
    void close_bet_round();
    bool one_player_left();
    void zero_bets_and_clear_actions();

private:
    bool m_stop;
    int m_min_bet;
    int m_max_bet;

    Wait m_wait;
    Deck& m_deck;
    Table& m_table;
    ActionOut& m_action_out;
    PlayersContainer& m_players;

    playerIterator m_turn;
    playerIterator m_open_player;
};


}// poker namespace
