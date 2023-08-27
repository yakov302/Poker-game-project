#pragma once

#include <thread>

#include "cli_cmd.hpp"
#include "card_round.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

namespace poker
{

class Game
{
public:
    Game(PlayersContainer& a_players, CardRound& a_card_round, ActionOut& a_action_out, playerIterator& a_game_open_player);
    ~Game();

    void run();
    void stop();
    void game_player_delete();
    void game_player_going_to_be_deleted(int a_client_socket);

private:
    void next();
    void next_it();
    void skip_viewers();
    bool chack_winer();
    void set_open_player();
    void pass_open_player_to_previous_player();

private:
    bool m_stop; 
    bool m_open_player_deleted;
    std::thread* m_thread;

    CardRound& m_card_round;
    ActionOut& m_action_out;
    PlayersContainer& m_players;
    playerIterator& m_game_open_player;
};


}// poker namespace
