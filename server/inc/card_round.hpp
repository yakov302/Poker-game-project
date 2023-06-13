#pragma once

#include "deck.hpp"
#include "table.hpp"
#include "winner.hpp"
#include "bet_round.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class CardRound
{
public:
    CardRound(PlayersContainer& a_players, Table& a_table, ActionOut& a_action_out, BetRound& a_bet_round);

    void run(playerIterator a_open_player);

private:
    void deal_cards();

    void bet(playerIterator a_open_player);
    void open_three_cards();
    void open_card();

    void close_card_round();
    void clear_hands();
    void table_clear_hand();
    void reset_players_variables();
    void reveal_cards_and_print_result();
    void pay_to_winner(std::string& a_winner);

    bool one_player_left(std::string& a_name);

private:
    bool m_bet;
    bool m_stop;

    Table& m_table;
    BetRound& m_bet_round;
    ActionOut& m_action_out;
    PlayersContainer& m_players;
};


}// poker namespace
