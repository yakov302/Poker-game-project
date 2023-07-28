#include "game.hpp"

namespace poker
{

extern std::string viewer;
extern bool dbg[NUM_OF_DBG_TYPES];

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
, m_card_round(a_card_round)
, m_action_out(a_action_out)
, m_players(a_players)
, m_open_player(m_players.begin())
{
    m_thread = new std::thread(impl::thread_function, this);
}

Game::~Game()
{
	m_thread->join();
	delete m_thread;
}

void Game::run()
{
    while(!m_stop)
    {
        if(m_players.num_of_players()< 2 || chack_winer())
        {
            m_players.wait().enter_wait();
            if(m_stop){break;}

            m_action_out.clear_text();
            set_open_player();
        }

        m_card_round.run(m_open_player);
        next();
    }
}

void Game::stop()
{
    m_stop = true;
    m_players.wait().exit_wait();
}

void Game::set_open_player()
{
    m_open_player = m_players.begin();
    skip_viewers();
}

bool Game::chack_winer()
{
    int count = 0;
    std::string name;
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_flags[viewer])
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

void Game::next_it()
{
    ++m_open_player;
    if(m_open_player == m_players.end())
        m_open_player = m_players.begin();
}

void Game::skip_viewers()
{
    while(m_open_player->second.get()->m_vars[viewer])
        next_it();
}

void Game::next()
{
    if(m_players.num_of_players() > 1)
    {
        next_it();
        skip_viewers();
    }
}


}// poker namespace
