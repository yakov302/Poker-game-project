#include "bet_round.hpp"

namespace poker
{

BetRound::BetRound(PlayersContainer& a_players, ActionOut& a_action_out, Table& a_table)
: m_stop(false)
, m_min_bet(0)
, m_max_bet(100000)
, m_wait()
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

        bet();

        if(one_player_left())
        {close_bet_round(); break;}

        next();
    }
}

bool BetRound::set_max_bet()
{
    auto it = m_players.begin();
    auto end = m_players.end();
    int min = 100000;

    while(it != end)
    {
        std::string name = it->second.get()->m_name;

        if(!m_players.is_flag_on(name, "fold")
        && !m_players.is_flag_on(name, "viewer")
        && m_players.is_it_has_a_cards(name))
        {
            if(m_players.amount(name) + m_players.bet(name) < min)
                min = m_players.amount(name) + m_players.bet(name);
        }
        ++it;
    }

    m_max_bet = min;

    if(m_max_bet == 0)
        return true;
    return false;
}

void BetRound::bet()
{
    if(m_turn->second.get()->m_fold 
    || m_turn->second.get()->m_viewer
    || m_turn->second.get()->m_hand.size() == 0)
        return;
        
    m_action_out.turn_on(m_turn->second.get()->m_name, "my_turn");
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

    if(m_turn->second->m_socket == a_client_socket)
        m_wait.exit_wait();
}

void BetRound::start_bet()
{
    m_action_out.start_bet(m_turn->second->m_name, m_turn->second->m_bet);
}

void BetRound::bet_in(int a_amount)
{
    if(m_turn->second->m_bet + a_amount > m_max_bet)
        m_action_out.invalid_bet_max(m_max_bet , m_turn->second->m_socket);

    else
    {
        m_turn->second->m_bet += a_amount;
        m_turn->second->m_amount -= a_amount;
        m_table.get_chip(a_amount);
        m_action_out.bet(m_turn->second->m_name, a_amount);
        m_action_out.table_get_chip(a_amount);
    }
}

void BetRound::finish_bet()
{
    if(m_turn->second->m_bet < m_min_bet)
        m_action_out.invalid_bet_min(m_min_bet , m_turn->second->m_socket);
 
    else
    {
        if(m_turn->second->m_bet > m_min_bet)
            m_open_player = m_turn;

        m_min_bet = m_turn->second->m_bet;
        m_action_out.turn_off(m_turn->second.get()->m_name, "my_turn");
        m_action_out.turn_off(m_turn->second.get()->m_name, "bet");
        m_wait.exit_wait();
    }
}

void BetRound::chack_in()
{
    if(m_min_bet > 0)
        m_action_out.invalid_bet_min(m_min_bet , m_turn->second->m_socket);
    
    else
    {
        m_action_out.turn_off(m_turn->second->m_name, "my_turn");
        m_action_out.check(m_turn->second->m_name);
        m_wait.exit_wait();
    }
}

void BetRound::fold_in()
{
    for(int i = 0; i < 2; ++i)
        m_players.give_card(m_turn->second->m_name);
    m_players.turn_on(m_turn->second->m_name, "fold");
    m_action_out.fold(m_turn->second->m_name);
    m_action_out.turn_off(m_turn->second->m_name, "my_turn");
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

        if(!m_players.is_flag_on(name, "fold")
        && !m_players.is_flag_on(name, "viewer")
        && m_players.is_it_has_a_cards(name))
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

        m_players.set_bet(name, 0);
        if(!m_players.is_flag_on(name, "fold"))
            m_action_out.clear_action(name);
        ++it;
    }
}


}// poker namespace

