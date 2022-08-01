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

Game::Game(PlayersContainer& a_players, CardRound& a_card_round, ActionOut& a_action_out)
: m_stop(false)
, m_players(a_players)
, m_card_round(a_card_round)
, m_action_out(a_action_out)
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
            set_open_player();
        }

        if(chack_winer())
            break;

        m_card_round.run(m_open_player);
        next();
    }
}

void Game::set_open_player()
{
    m_open_player = m_players.begin();
    while(m_open_player->second.get()->m_viewer)
        ++m_open_player;
}

void Game::next()
{
    ++m_open_player;
    if(m_open_player == m_players.end())
        m_open_player = m_players.begin();
        
    while(m_open_player->second.get()->m_viewer)
    {
        ++m_open_player;
        if(m_open_player == m_players.end())
            m_open_player = m_players.begin();
    }
}


bool Game::chack_winer()
{
    int count = 0;
    auto it = m_players.begin();
    auto end = m_players.end();
    std::string name;

    while(it != end)
    {
        if(!it->second.get()->m_viewer)
        {
            name = it->second.get()->m_name;
            ++count;
        }
        
        if(count > 1)
            return false;

        ++it;
    }

    m_action_out.game_winer(name);
    return true; 
}


}// poker namespace
