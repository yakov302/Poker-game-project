#include "players_container.hpp"

namespace poker
{

extern Deck deck;

PlayersContainer::PlayersContainer(ActionOut& a_action_out)
: m_wait()
, m_action_out(a_action_out)
, m_players()
{
    
}

void PlayersContainer::new_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket)
{
    if(m_players.find(a_name) != m_players.end())
        return;

    //IF TABLE FULL - 6 PLYERS ???

    m_players[a_name] = playerPointer(new Player(a_name, a_gender, a_amount, a_client_socket));
    m_action_out.log_in_success(a_name, a_gender, a_client_socket);
    m_action_out.get_player(a_name, a_gender, a_amount);

    for(auto player: m_players)
        m_action_out.get_player(player.second->m_name, player.second->m_gender, player.second->m_amount, a_client_socket);

    if(num_of_players() > 1)
        m_wait.exit_wait();
}

void PlayersContainer::delete_player(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_action_out.delete_player(a_name);
    for(int i = 0; i < 2; ++i)
        deck.push_card(give_card(a_name));
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

void PlayersContainer::get_card(std::string& a_name, cardPointer a_card)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_hand.emplace_back(a_card);
}

cardPointer PlayersContainer::give_card(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end() ||
       m_players[a_name]->m_hand.empty())
        return nullptr;

    int index = m_players[a_name]->m_hand.size()-1;
    cardPointer card = m_players[a_name]->m_hand[index];
    m_players[a_name]->m_hand.pop_back();
    return card;
}

void PlayersContainer::turn_on(std::string& a_name, std::string a_flag)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    if(a_flag == "my_turn")
        m_players[a_name]->m_my_turn = true;

    if(a_flag == "fold")
        m_players[a_name]->m_fold = true;

    if(a_flag == "viewer")
        m_players[a_name]->m_viewer = true;
}

void PlayersContainer::turn_off(std::string& a_name, std::string a_flag)
{
    if(m_players.find(a_name) == m_players.end())
        return;
    
    if(a_flag == "my_turn")
        m_players[a_name]->m_my_turn = false;
    
    if(a_flag == "fold")
        m_players[a_name]->m_fold = false;
    
    if(a_flag == "viewer")
        m_players[a_name]->m_viewer = false;
}

bool PlayersContainer::is_flag_on(std::string& a_name, std::string a_flag)
{
    if(a_flag == "my_turn")
        return m_players[a_name]->m_my_turn;
    
    if(a_flag == "fold")
        return m_players[a_name]->m_fold;
    
    if(a_flag == "viewer")
        return m_players[a_name]->m_viewer;

    return false;
}

bool PlayersContainer::is_it_has_a_cards(std::string& a_name)
{


void PlayersContainer::set_bet(std::string& a_name, int a_amount)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_bet = a_amount;
}

void PlayersContainer::set_result(std::string& a_name, int a_result)
{
    if(m_players.find(a_name) == m_players.end())
        return;

    m_players[a_name]->m_result = a_result;
}

int PlayersContainer::bet(std::string& a_name)
{
    return m_players[a_name]->m_bet;
}

int PlayersContainer::result(std::string& a_name)
{
    return m_players[a_name]->m_result;
}

int PlayersContainer::amount(std::string& a_name)
{
    return m_players[a_name]->m_amount;
}

cardPointer PlayersContainer::first_card(std::string& a_name)
{
    return m_players[a_name]->m_hand[0];
}

cardPointer PlayersContainer::second_card(std::string& a_name)
{
      return m_players[a_name]->m_hand[1];
}

Wait& PlayersContainer::wait()
{
    return m_wait;
}

playerIterator PlayersContainer::begin()
{
    return  m_players.begin();
}

playerIterator PlayersContainer::end()
{
    return m_players.end();
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


}// poker namespace
