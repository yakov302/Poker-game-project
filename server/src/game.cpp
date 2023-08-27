#include "game.hpp"

namespace poker
{

extern std::string socket;
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

Game::Game(PlayersContainer& a_players, CardRound& a_card_round, ActionOut& a_action_out, playerIterator& a_game_open_player)
: m_stop(false)
, m_open_player_deleted(false)
, m_card_round(a_card_round)
, m_action_out(a_action_out)
, m_players(a_players)
, m_game_open_player(a_game_open_player)
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
    m_game_open_player = m_players.begin();

    if(dbg[GAME] && (m_players.num_of_players() > 0))[[unlikely]]
        std::cout << __func__ << "(): game set m_game_open_player = (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket] << ")" << std::endl;

    while(!m_stop)
    {
        if(m_players.num_of_players()< 2 || chack_winer())
        {
            if(dbg[GAME])[[unlikely]]
                std::cout << __func__ << "(): game enter wait" << std::endl;
            
            m_players.wait().enter_wait();
            if(m_stop){break;}

            if(dbg[GAME])[[unlikely]]
                std::cout << __func__ << "(): game exit wait" << std::endl;

            m_action_out.clear_text();
            set_open_player();
        }

        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): game call card round with open_player (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket] << ")" << std::endl;

        m_card_round.run();
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
    m_game_open_player = m_players.begin();

    if(dbg[GAME])[[unlikely]]
        std::cout << __func__ << "(): game set m_game_open_player = (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket] << ")" << std::endl;
        
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
    if(dbg[GAME])[[unlikely]]
        std::cout << __func__ << "(): exists m_game_open_player: " << m_game_open_player->second->m_name << std::endl;

    ++m_game_open_player;
    if(m_game_open_player == m_players.end())
        m_game_open_player = m_players.begin();

    if(dbg[GAME])[[unlikely]]
        std::cout << __func__ << "(): new m_game_open_player: " << m_game_open_player->second->m_name << std::endl;
}

void Game::skip_viewers()
{
    while(m_game_open_player->second.get()->m_vars[viewer])
    {
        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): m_game_open_player " << m_game_open_player->second->m_name << " is viewer call next_it " << std::endl;
        
        next_it();
    }
}

void Game::next()
{
    if(m_players.num_of_players() > 1)
    {
        if(dbg[GAME])[[unlikely]]
            m_players.print_players();
        
        next_it();
        skip_viewers();

        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): game made ++m_game_open_player (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket] << ")" << std::endl;
    }
}

void Game::game_player_delete()
{
    if(m_players.num_of_players() == 0)
    {
        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): table empty no players exists - > return" << std::endl;

        return;
    }

    if(m_open_player_deleted) 
    {        
        next_it();
        m_open_player_deleted = false;
        
        if(dbg[GAME])[[unlikely]]
        {
            m_players.print_players();
            std::cout << __func__ << "(): m_open_player_deleted flag is on -> moves m_game_open_player to the next player (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket] << ")" << std::endl;
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
    if((m_players.num_of_players() < 1))
    {
        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): table empty no players exists -> return" << std::endl;

        return;
    }

    if(m_game_open_player->second->m_vars[socket] == a_client_socket)
    {
        if(dbg[GAME])[[unlikely]]
        {
            m_players.print_players();
            std::cout << __func__ << "(): m_game_open_player (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket]<< ") is the deleted player" << std::endl;
        }

        m_open_player_deleted = true;
        pass_open_player_to_previous_player();

        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): m_game_open_player pass to previous player (" << m_game_open_player->second->m_name << ", " << m_game_open_player->second->m_vars[socket] << ")" << std::endl;
    }
    else
    {
        if(dbg[GAME])[[unlikely]]
            std::cout << __func__ << "(): socket " << a_client_socket << "is not the open player -> do nothing" << std::endl;
    }
}


}// poker namespace
