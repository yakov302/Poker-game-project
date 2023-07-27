#include "bet_round.hpp"

namespace poker
{

extern std::string bet;
extern std::string fold;
extern std::string amount;
extern std::string viewer;
extern std::string socket;
extern std::string my_turn;

namespace impl
{

extern bool active_player_with_card(PlayersContainer& a_players, std::string& name);


}//impl namespace

BetRound::BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table, Deck& a_deck)
: m_stop(false)
, m_min_bet(0)
, m_max_bet(0)
, m_wait()
, m_deck(a_deck)
, m_table(a_table)
, m_action_out(a_action_out)
, m_players(a_players)
, m_turn(a_players.begin())
, m_open_player(a_players.begin())
{

}

void BetRound::run(playerIterator a_open_player)
{
    m_open_player = a_open_player;
    m_turn = m_open_player;
    m_stop = false;

    while(!m_stop)
    {
        if(set_max_bet())
        {close_bet_round(); break;}

        wait_for_bet();

        if(one_player_left())
        {close_bet_round(); break;}

        next();
    }
}

bool BetRound::set_max_bet()
{
    auto it = m_players.begin();
    auto end = m_players.end();
    int min = HUGE_NUMBER;

    while(it != end)
    {
        std::string name = it->second.get()->m_name;

        if(impl::active_player_with_card(m_players, name))
        {
            if(m_players.get(name, amount) + m_players.get(name, bet) < min)
                min = m_players.get(name, amount) + m_players.get(name, bet);
        }
        ++it;
    }

    m_max_bet = min;

    if(m_max_bet == 0)
        return true;
    return false;
}

void BetRound::wait_for_bet()
{
    if(!impl::active_player_with_card(m_players, m_turn->second.get()->m_name))
        return;
        
    m_action_out.turn_on(m_turn->second.get()->m_name, my_turn);
    m_wait.enter_wait();
}

void BetRound::next()
{
    ++m_turn;
    if(m_turn == m_players.end())   
        m_turn = m_players.begin();

    if(m_turn == m_open_player)
        close_bet_round();
}

void BetRound::player_deleted(int a_client_socket)
{
    if(m_players.num_of_players() < 2)
    {
        m_wait.exit_wait();
        return;
    }

    if(m_turn->second->m_vars[socket] == a_client_socket)
        m_wait.exit_wait();
}

void BetRound::start_bet()
{
    m_action_out.start_bet(m_turn->second->m_name, m_turn->second->m_vars[bet]);
}

void BetRound::bet_in(int a_amount)
{
    if(m_turn->second->m_vars[bet] + a_amount > m_max_bet)
        m_action_out.invalid_bet_max(m_max_bet , m_turn->second->m_vars[socket]);

    else
    {
        m_turn->second->m_vars[bet] += a_amount;
        m_turn->second->m_vars[amount]-= a_amount;
        m_table.get_chip(a_amount);
        m_action_out.bet(m_turn->second->m_name, a_amount);
        m_action_out.table_get_chip(a_amount);
    }
}

void BetRound::finish_bet()
{
    if(m_turn->second->m_vars[bet] < m_min_bet)
        m_action_out.invalid_bet_min(m_min_bet , m_turn->second->m_vars[socket]);
 
    else
    {
        if(m_turn->second->m_vars[bet] > m_min_bet)
            m_open_player = m_turn;

        m_min_bet = m_turn->second->m_vars[bet];
        m_action_out.turn_off(m_turn->second.get()->m_name, my_turn);
        m_action_out.turn_off(m_turn->second.get()->m_name, bet);
        m_wait.exit_wait();
    }
}

void BetRound::chack_in()
{
    if(m_min_bet > 0)
        m_action_out.invalid_bet_min(m_min_bet , m_turn->second->m_vars[socket]);
    
    else
    {
        m_action_out.turn_off(m_turn->second->m_name, my_turn);
        m_action_out.check(m_turn->second->m_name);
        m_wait.exit_wait();
    }
}

void BetRound::fold_in()
{
    for(int i = 0; i < 2; ++i)
        m_deck.push_card(m_players.give_card(m_turn->second->m_name));
    m_players.turn_on(m_turn->second->m_name, fold);
    m_action_out.fold(m_turn->second->m_name);
    m_action_out.turn_off(m_turn->second->m_name, my_turn);
    usleep(1000000);
    m_wait.exit_wait();
}

bool BetRound::one_player_left()
{
    int count = 0;
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name = it->second.get()->m_name;

        if(impl::active_player_with_card(m_players, name))
            ++count;

        if(count > 1)
            return false;

        ++it;
    }

    return true; 
}

void BetRound::close_bet_round()
{
    m_stop = true; 
    m_min_bet = 0;
    zero_bets_and_clear_actions();
}

void BetRound::zero_bets_and_clear_actions()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        std::string name = it->second.get()->m_name;

        m_players.set(name, bet, 0);
        if(!m_players.is_flag_on(name, fold))
            m_action_out.clear_action(name);
        ++it;
    }
}


}// poker namespace

