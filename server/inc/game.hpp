#pragma once

#include <thread>

#include "card_round.hpp"
#include "action_out.hpp"
#include "players_container.hpp"

namespace poker
{

class Game
{
public:
    Game(PlayersContainer& a_players, CardRound& a_card_round, ActionOut& a_action_out);
    ~Game();

    void run();
    void stop();
    
private:
    void next();
    bool chack_winer();
    void set_open_player();

private:
    bool m_stop; 
    std::thread* m_thread;

    CardRound& m_card_round;
    ActionOut& m_action_out;
    PlayersContainer& m_players;

    playerIterator m_open_player;
};


}// poker namespace
