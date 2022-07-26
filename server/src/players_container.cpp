#include "players_container.hpp"
#include <iostream>


namespace poker
{

PlayersContainer::PlayersContainer(ActionOut& a_action_out)
: m_action_out(a_action_out)
, m_players()
{
    
}

int PlayersContainer::num_of_players()const
{
    return m_players.size();
}

bool PlayersContainer::log_in_chack(std::string a_name,  int a_client_socket)const
{
    if(m_players.find(a_name) != m_players.end())
    {
        m_action_out.user_name_alredy_log(a_client_socket);
        return false;
    }
    return true;
}

playerIterator PlayersContainer::begin()
{
    return  m_players.begin();
}

playerIterator PlayersContainer::end()
{
    return m_players.end();
}

void PlayersContainer::new_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket)
{
    if(m_players.find(a_name) != m_players.end())
        return;

    m_action_out.log_in_success(a_name, a_gender, a_client_socket);
    m_action_out.get_player(a_name, a_gender, a_amount);

    for(auto player: m_players)
        m_action_out.get_player(player.second->m_name, player.second->m_gender, player.second->m_amount, a_client_socket);

    m_players[a_name] = playerPointer(new Player(a_name, a_gender, a_amount, a_client_socket));
}

void PlayersContainer::delete_player(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_action_out.delete_player(a_name);
    m_players.erase(a_name);
}

void PlayersContainer::delete_player(int a_client_socket)
{
    for(auto player : m_players)
    {
        if(player.second.get()->m_socket == a_client_socket)
        {
            delete_player(player.second.get()->m_name);
            break;
        }
    }
}

void PlayersContainer::decrease (std::string& a_name, int a_amount)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_amount -= a_amount;
}

void PlayersContainer::increase (std::string& a_name, int a_amount)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_amount += a_amount;
}

void PlayersContainer::get_card(std::string& a_name, Card a_card)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_hand.emplace_back(a_card);
}

void PlayersContainer::clear_hand(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_hand.clear();
}

void PlayersContainer::turn_on(std::string& a_name, std::string a_flag)
{
    if(m_players.find(a_name) == m_players.end())
        return;
    
    if(a_flag == "my_turn")
        m_players[a_name]->m_my_turn = true;
}

void PlayersContainer::turn_off(std::string& a_name, std::string a_flag)
{
    if(m_players.find(a_name) == m_players.end())
        return;
    
    if(a_flag == "my_turn")
        m_players[a_name]->m_my_turn = false;
}


}// poker namespace
