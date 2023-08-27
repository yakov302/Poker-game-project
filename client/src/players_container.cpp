#include "players_container.hpp"

namespace poker
{

namespace impl
{

void set_locations(std::vector<std::string>& a_locations, int a_num_of_players)
{
    a_locations.reserve(5);
    for(int i = 0; i < a_num_of_players; ++i)
        a_locations.emplace_back("empty");
}


}// impl namespace

PlayersContainer::PlayersContainer()
: m_num_of_players(0)
, m_locations()
, m_players()
{
    impl::set_locations(m_locations, MAX_NUM_OF_PAYERS_IN_TABLE);
} 

void PlayersContainer::get_player(std::string& a_name, std::string& a_gender, int a_amount)
{
    if(m_players.find(a_name) != m_players.end())[[unlikely]]
        return;

    int i = 0;
    find_empty_place(i, a_name);
    int player_x_pos = i*PLAYER_SIZE + PLAYERS_X_POS;
    int card_x_pos = player_x_pos + GAP_BETWEEN_CARDS_AND_PLAYER; 

    m_players[a_name] = playerPointer(new Player(a_name, a_gender, a_amount, player_x_pos,  PLAYERS_Y_POS, card_x_pos, CARD_Y_POS, CARDS_SCLAE, GAP_BETWEEN_FRONT_CARDS_, GAP_BETWEEN_BACK_CARDS_));
    ++m_num_of_players;
}

void PlayersContainer::find_empty_place(int& a_i, std::string& a_name)
{
    while(m_locations[a_i] != "empty")
        ++a_i;

    m_locations[a_i] = a_name;
}

void PlayersContainer::get_player(std::string& a_name, Self& a_player)
{
    if(m_players.find(a_name) != m_players.end())[[unlikely]]
        return;
        
    m_players[a_name] = playerPointer(&a_player);
    ++m_num_of_players;
}

void PlayersContainer::delete_player(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players.erase(a_name);
    set_as_empty_place(a_name, 5);
    --m_num_of_players;
}

void PlayersContainer::set_as_empty_place(std::string& a_name, int num_of_players)
{
    for(int i = 0; i < num_of_players; ++i)
    {
        if(m_locations[i] == a_name)
        {
            m_locations[i] = "empty";
            break;
        }
    }
}

void PlayersContainer::get_chip(std::string& a_name, int a_chip)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name].get()->get_chip(a_chip);
}

void PlayersContainer::bet(std::string& a_name, int a_amount)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name].get()->bet(a_amount);
}

void PlayersContainer::get_card(std::string& a_name, std::string& a_suit, int a_number)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name].get()->get_card(a_suit, a_number);
}

void PlayersContainer::give_card(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name].get()->give_card();
}

void PlayersContainer::set_action(std::string& a_name, std::string a_action)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name].get()->set_action(a_action);
}

void PlayersContainer::update_current_bet(std::string& a_name, int a_amount)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;
    
    m_players[a_name].get()->update_current_bet(a_amount);
}

void PlayersContainer::turn_on_flag(std::string& a_name, std::string& a_flag)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name].get()->turn_on_flag(a_flag);
}

void PlayersContainer::turn_off_flag(std::string& a_name, std::string& a_flag)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
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

void PlayersContainer::clear(std::string& a_self_name)
{
    int condition;
    if(m_players.find(a_self_name) != m_players.end())
        condition = 1;
    else
        condition = 0;

    while(m_num_of_players > condition)
    {
        for(auto player : m_players)
        {
            std::string name = player.first;
            if(name != a_self_name)
            {
                delete_player(name);
                break;
            }
        }
    }
}

int PlayersContainer::num_of_players()
{
    return m_num_of_players;
}


}// poker namespace
