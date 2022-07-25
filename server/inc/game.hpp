#pragma once

#include <thread>

#include "players_container.hpp"
#include "card_round.hpp"


namespace poker
{

class Game
{
public:
    Game(PlayersContainer& a_players, CardRound& a_card_round);

    void run();
    void next();

private:
    bool m_stop;
    std::thread* m_thread;
    PlayersContainer& m_players;
    CardRound& m_card_round;
    playerIterator m_open_player;

};


}// poker namespace
