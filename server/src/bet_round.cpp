#include "bet_round.hpp"

#include <iostream>

namespace poker
{

BetRound::BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table)
: m_min_bet(0)
, m_stop(false)
, m_wait_for_bet(false)
, m_mutex()
, m_cond_var()
, m_players(a_players)
, m_action_out(a_action_out)
, m_table(a_table)
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
        bet();
        if(one_player_left())
        {
            close_bet_round();
            break;
        }
        next();
    }
}

void BetRound::bet()
{
    if(m_turn->second.get()->m_fold)
        return;
        
    m_action_out.turn_on(m_turn->second.get()->m_name, "my_turn");
    enter_wait();
}

void BetRound::enter_wait()
{
    Lock lock(m_mutex);
    m_wait_for_bet = true;
    std::cout << "----enter wait----\n";
    m_cond_var.wait(lock, [this]() {return !m_wait_for_bet;});
    std::cout << "----exit wait----\n";
    m_cond_var.notify_all();
}

void BetRound::exit_wait()
{
    Lock lock(m_mutex);
    std::cout << "----enter wait----\n";
    m_cond_var.wait(lock, [this]() {return m_wait_for_bet;});
    std::cout << "----exit wait----\n";
    m_wait_for_bet = false;
    m_cond_var.notify_all();
}

void BetRound::next()
{
    ++m_turn;

    if(m_turn == m_players.end())
        m_turn = m_players.begin();

    if(m_turn == m_open_player)
        close_bet_round();
}

void BetRound::start_bet()
{
    m_action_out.start_bet(m_turn->second->m_name, m_turn->second->m_bet);
}

void BetRound::bet_in(int a_amount)
{
    m_turn->second->m_bet += a_amount;
    m_turn->second->m_amount -= a_amount;
    m_table.get_chip(a_amount);
    m_action_out.bet(m_turn->second->m_name, a_amount);
    m_action_out.table_get_chips(a_amount);
}

void BetRound::finish_bet()
{
    if(m_turn->second->m_bet < m_min_bet)
        m_action_out.invalid_bet(m_min_bet , m_turn->second->m_socket);
 
    else
    {
        if(m_turn->second->m_bet > m_min_bet)
            m_open_player = m_turn;

        exit_wait();
        m_min_bet = m_turn->second->m_bet;
        m_action_out.turn_off(m_turn->second.get()->m_name, "my_turn");
        m_action_out.turn_off(m_turn->second.get()->m_name, "bet");
    }
}

void BetRound::chack_in()
{
    if(m_min_bet > 0)
        m_action_out.invalid_bet(m_min_bet , m_turn->second->m_socket);
    
    else
    {
        exit_wait();
        m_action_out.turn_off(m_turn->second->m_name, "my_turn");
        m_action_out.check(m_turn->second->m_name);
    }
}

void BetRound::fold_in()
{
    m_players.turn_on(m_turn->second->m_name, "fold");
    m_action_out.fold(m_turn->second->m_name);
    m_action_out.turn_off(m_turn->second->m_name, "my_turn");
    exit_wait();
}

bool BetRound::one_player_left()
{
    int count = 0;
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        if(!it->second.get()->m_fold)
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
    m_wait_for_bet = false;
    zero_bets_and_clear_actions();
}

void BetRound::zero_bets_and_clear_actions()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    while(it != end)
    {
        it->second.get()->m_bet = 0;
        m_action_out.clear_action(it->second.get()->m_name);
        ++it;
    }
}


}// poker namespace
