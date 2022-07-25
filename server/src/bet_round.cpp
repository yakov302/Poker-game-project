#include "bet_round.hpp"

namespace poker
{

BetRound::BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table)
: m_min_bet(0)
, m_current_bet(0)
, m_stop(false)
, m_wait_for_bet(false)
, m_players(a_players)
, m_action_out(a_action_out)
, m_table(a_table)
, m_open_player(a_players.begin())
, m_turn(a_players.begin())
{

}

void BetRound::bet_in(int a_amount)
{
    m_current_bet += a_amount; 
    m_table.get_chip(a_amount);
    m_action_out.bet(m_turn->second->m_name, a_amount);
    m_action_out.table_get_chips(a_amount);
}

void BetRound::finish_bet()
{
    if(m_current_bet < m_min_bet)
        m_action_out.invalid_bet(m_min_bet, m_turn->second->m_socket);

    else
    {
        if(m_current_bet > m_min_bet)
            m_open_player = m_turn;

        m_wait_for_bet = false;
        m_min_bet = m_current_bet;
        m_current_bet = 0;
        m_action_out.turn_off(m_turn->second.get()->m_name, "my_turn");
        m_action_out.turn_off(m_turn->second.get()->m_name, "bet");
    }
}

void BetRound::run(playerIterator a_open_player)
{
    m_open_player = a_open_player;
    m_turn = m_open_player;
    m_stop = false;

    while(!m_stop)
    {
        bet();
        next();
    }
}

void BetRound::bet()
{
    m_action_out.turn_on(m_turn->second.get()->m_name, "my_turn");
    m_wait_for_bet = true;
    while(m_wait_for_bet)
    {//cond var
    }
}

void BetRound::next()
{
    ++m_turn;
    if(m_turn == m_players.end())
        m_turn = m_players.begin();
    
    if(m_turn == m_open_player)
        close_bet_round();
}

void BetRound::close_bet_round()
{
    m_stop = true; 
    m_min_bet = 0;
    m_current_bet = 0;
    m_wait_for_bet = false;
}

}// poker namespace
