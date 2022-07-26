#include "bet_round.hpp"
#include <iostream>

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
    std::cout << "bet in\n";
    m_current_bet += a_amount; 
    m_table.get_chip(a_amount);
    m_action_out.bet(m_turn->second->m_name, a_amount);
    m_action_out.table_get_chips(a_amount);
}

void BetRound::finish_bet()
{
    std::cout << "finish_bet\n";
    if(m_current_bet < m_min_bet)
    {
        m_action_out.invalid_bet(m_min_bet, m_turn->second->m_socket);
        std::cout << "invalid_bet: " << m_turn->second->m_name<<"\n";
    }

    else
    {
        if(m_current_bet > m_min_bet)
            m_open_player = m_turn;

        m_wait_for_bet = false;
        m_min_bet = m_current_bet;
        m_current_bet = 0;
        std::cout << "turn_off my turn : " << m_turn->second->m_name<<"\n";
        m_action_out.turn_off(m_turn->second.get()->m_name, "my_turn");
        m_action_out.turn_off(m_turn->second.get()->m_name, "bet");
    }
}

void BetRound::run(playerIterator a_open_player)
{
    std::cout << "enter run\n";
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
    std::cout << "enter bet\n";
    std::cout << "eturn_on my turn :" << m_turn->second.get()->m_name << "\n";
    m_action_out.turn_on(m_turn->second.get()->m_name, "my_turn");
    m_wait_for_bet = true;
    std::cout << "wait_for_bet\n";
    while(m_wait_for_bet)
    {
        
    }
    std::cout << "stop wait_for_bet\n";
}

void BetRound::next()
{
    std::cout << "enntered next\n";
    std::cout << "curent player: " << m_turn->second.get()->m_name<<"\n";
    ++m_turn;
    if(m_turn == m_players.end())
        m_turn = m_players.begin();
    std::cout << "next player: " << m_turn->second.get()->m_name<<"\n";
    std::cout << "open player: " << m_open_player->second.get()->m_name<<"\n";

    if(m_turn == m_open_player)
        close_bet_round();
}

void BetRound::close_bet_round()
{
    std::cout << "close_bet_round: " << "\n";

    m_stop = true; 
    m_min_bet = 0;
    m_current_bet = 0;
    m_wait_for_bet = false;
}

}// poker namespace
