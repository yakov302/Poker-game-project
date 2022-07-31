#pragma once

#include <thread>

#include "players_container.hpp"
#include "card_round.hpp"


namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class Game
{
public:
    Game(PlayersContainer& a_players, CardRound& a_card_round);

    void run();
    void next();
    void wait();

private:
    bool m_stop;
    
    std::mutex m_mutex;
    std::thread* m_thread;
    PlayersContainer& m_players;
    CardRound& m_card_round;
    playerIterator m_open_player;

};


}// poker namespace
