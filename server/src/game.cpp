#include "game.hpp"
#include <iostream>

namespace poker
{

namespace impl
{

static void* thread_function(void* arg)
{
	Game* game = static_cast<Game*> (arg);
	game->run();
	return 0;
}


} //namespace impl

Game::Game(PlayersContainer& a_players, CardRound& a_card_round)
: m_stop(false)
, m_players(a_players)
, m_card_round(a_card_round)
, m_open_player(m_players.begin())
{
    m_thread = new std::thread(impl::thread_function, this);
}

void Game::run()
{
    while (!m_stop)
    {
        if(m_players.num_of_players()< 2)
        {
            m_players.wait().enter_wait();
            m_open_player = m_players.begin();
        }
        
        m_card_round.run(m_open_player);
        next();
    }
}

void Game::next()
{
    ++m_open_player;
    if(m_open_player == m_players.end())
        m_open_player = m_players.begin();
}


// void Game::wait()
// {
//     Lock lock(m_mutex);
//     std::cout << "----enter game wait----\n";
//     m_players.cond_var().wait(lock, [this]() {return m_players.num_of_players() > 1;});
//     std::cout << "----exit game wait----\n";
// }

}// poker namespace
