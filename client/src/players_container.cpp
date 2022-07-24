#include "players_container.hpp"

namespace poker
{

PlayersContainer::PlayersContainer()
: m_num_of_players(0)
, m_players()
{

}

void PlayersContainer::get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_of_printing)
{
    if(m_players.find(a_name) != m_players.end())
        return;

    if(a_x_self == 0)
        a_x_self = (m_num_of_players-1)*300 + 230;

    if(a_x_card == 0)
        a_x_card = a_x_self + 140;

    if(a_y_card == 0)   
        a_y_card = a_y_self + 90;

    m_players[a_name] = playerPointer(new Player(a_name, a_gender, a_amount, a_x_self,  a_y_self, a_x_card, a_y_card, a_scale_card, a_gap_of_printing));
    ++m_num_of_players;
}

void PlayersContainer::get_player(std::string& a_name, Self& a_player)
{
    if(m_players.find(a_name) != m_players.end())
        return;
        
    m_players[a_name] = playerPointer(&a_player);
    ++m_num_of_players;
}

void PlayersContainer::delete_player(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players.erase(a_name);
    --m_num_of_players;
}

void PlayersContainer::get_chips(std::string& a_name, std::vector<int>& a_chips)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->get_chips(a_chips);
}

void PlayersContainer::bet(std::string& a_name, int a_amount)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->bet(a_amount);
}

void PlayersContainer::get_card(std::string& a_name, std::string& a_suit, int a_number)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->get_card(a_suit, a_number);
}

void PlayersContainer::clear_hand(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->clear_hand();
}

void PlayersContainer::set_action(std::string& a_name, std::string a_action)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->set_action(a_action);
}

void PlayersContainer::turn_on_flag(std::string& a_name, std::string a_flag)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->turn_on_flag(a_flag);
}

void PlayersContainer::turn_off_flag(std::string& a_name, std::string a_flag)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name].get()->turn_off_flag(a_flag);
}

void PlayersContainer::draw_Players(sf::RenderWindow& a_window)
{
    for(auto player : m_players)
    {
        player.second.get()->draw_player(a_window);
    }
}


}// poker namespace
