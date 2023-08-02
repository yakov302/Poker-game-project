# pragma once

#include "game.hpp"
#include "deck.hpp"
#include "table.hpp"
#include "cli_cmd.hpp"
#include "bet_round.hpp"
#include "card_round.hpp"
#include "players_container.hpp"

namespace poker
{

class TableManager
{
public:
    TableManager(TcpServer& a_tcp);

    bool is_table_full();
    bool is_table_empty();

    void player_deleted();
    void player_going_to_be_deleted(int a_client_socket);

private:
    friend class ActionIn;
    friend class TablesContainer;

    Deck m_deck;
    Table m_table;
    ActionOut m_actio_out;
    PlayersContainer m_players;
    BetRound m_bet_round;
    CardRound m_card_round;
    Game m_game;
};


}// poker namespace
