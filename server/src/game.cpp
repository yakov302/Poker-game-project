#include "game.hpp"

namespace poker
{

extern std::string socket;
extern std::string viewer;

Wait wait_to_m_turn;
playerIterator open_player;

extern bool m_turn_deleted;
bool open_player_deleted = false;

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
    open_player = m_players.begin();

    while(!m_stop)
    {
        if(m_players.num_of_players()< 2 || chack_winer())
        {
            m_players.wait().enter_wait();
            if(m_stop){break;}

            m_action_out.clear_text();
            set_open_player();
        }

        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): game call card round with open_player (" << open_player->second->m_name << ", " << open_player->second->m_vars[socket] << ")" << std::endl;

        m_card_round.run();
        next();
    }
}

void Game::stop()
{
    m_stop = true;
    wait_to_m_turn.exit_wait();
    m_players.wait().exit_wait();
}

void Game::set_open_player()
{
    open_player = m_players.begin();
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
    ++open_player;
    if(open_player == m_players.end())
        open_player = m_players.begin();
}

void Game::skip_viewers()
{
    while(open_player->second.get()->m_vars[viewer])
        next_it();
}

void Game::next()
{
    if(m_players.num_of_players() > 1)
    {
        next_it();
        skip_viewers();

        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): game made ++open_player (" << open_player->second->m_name << ", " << open_player->second->m_vars[socket] << ")" << std::endl;
    }
}

void Game::game_player_delete()
{
    if(m_players.num_of_players() == 0)
        return;

    if(open_player_deleted) 
    {
        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): wait_to_m_turn.enter_wait()" << std::endl;
        
        if(m_turn_deleted)
            wait_to_m_turn.enter_wait();
        
        next_it();
        open_player_deleted = false;
        
        if(dbg[GAME])[[unlikely]]
        {
            m_players.print_players();
            std::cout << __func__ << "(): open_player_deleted flag is on -> moves open_player to the next player (" << open_player->second->m_name << ", " << open_player->second->m_vars[socket] << ")" << std::endl;
        }
    }
}

void Game::pass_open_player_to_previous_player()
{  
    int num_of_steps = m_players.num_of_players() - 1;
    for(int i = 0; i < num_of_steps; ++i)
        next_it();
}

void Game::game_player_going_to_be_deleted(int a_client_socket)
{
    if(open_player->second->m_vars[socket] == a_client_socket)
    {
        if(dbg[GAME])[[unlikely]]
        {
            m_players.print_players();
            std::cout << __func__ << "(): open_player (" << open_player->second->m_name << ", " << open_player->second->m_vars[socket]<< ") is the deleted player" << std::endl;
        }

        open_player_deleted = true;
        pass_open_player_to_previous_player();

        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): open_player pass to previous player (" << open_player->second->m_name << ", " << open_player->second->m_vars[socket] << ")" << std::endl;
    }
}


}// poker namespace
