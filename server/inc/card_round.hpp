#pragma once

#include "players_container.hpp"
#include "deck.hpp"
#include "table.hpp"
#include "action_out.hpp"
#include "bet_round.hpp"
#include "winner.hpp"


namespace poker
{

class CardRound
{
public:
    CardRound(PlayersContainer& a_players, Table& a_table, ActionOut& a_action_out, BetRound& a_bet_round);

    void run(playerIterator a_open_player);

private:
    void deal_cards();
    void turn_off_folds();
    void bet(playerIterator a_open_player);
    void open_three_cards();
    void open_card();
    void close_card_round();
    void reveal_cards();
    void clear_hands();
    void turn_off_reveal_cards();
    void chack_money();
    void print_result();
    void clear_actions();
    void table_clear_hand();
    void pay_to_winner(std::string& a_winner);
    bool one_player_left();
    std::string one_player();

private:
    bool m_stop;
    bool m_bet;

    Deck m_deck;
    PlayersContainer& m_players;
    Table& m_table;
    ActionOut& m_action_out;
    BetRound& m_bet_round;
};


}// poker namespace
