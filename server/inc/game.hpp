#pragma once

#include <thread>

#include "players_container.hpp"
#include "card_round.hpp"
#include "action_out.hpp"



namespace poker
{

class Game
{
public:
    Game(PlayersContainer& a_players, CardRound& a_card_round, ActionOut& a_action_out);

    void run();
    
private:
    void next();
    void set_open_player();
    bool chack_winer();

private:
    bool m_stop; 
    std::thread* m_thread;
    PlayersContainer& m_players;
    CardRound& m_card_round;
    ActionOut& m_action_out;
    playerIterator m_open_player;
};


}// poker namespace
