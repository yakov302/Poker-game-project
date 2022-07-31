#pragma once

#include "players_container.hpp"
#include "deck.hpp"
#include "table.hpp"
#include "action_out.hpp"
#include "bet_round.hpp"

namespace poker
{

class CardRound
{
public:
    CardRound(PlayersContainer& a_players, Table& a_table, ActionOut& a_action_out, BetRound& a_bet_round);

    void run(playerIterator a_open_player);

private:
    bool one_player_left();
    std::string one_player();
    void turn_off_folds();
    void deal_cards();
    void bet(playerIterator a_open_player);
    void open_three_cards();
    void open_card();
    void close_card_round();
    void reveal_cards();
    std::string chack_winer();

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