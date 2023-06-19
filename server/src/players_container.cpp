#include "players_container.hpp"

namespace poker
{

extern Deck deck;
std::string bet = "m_bet";
std::string fold = "m_fold";
std::string viewer = "m_viewer";
std::string amount = "m_amount";
std::string result = "m_result";
std::string my_turn = "m_my_turn";

PlayersContainer::PlayersContainer(ActionOut& a_action_out)
: m_wait()
, m_action_out(a_action_out)
, m_players()
{
    m_players.reserve(MAX_NUM_OF_PLAYERS);
}

void PlayersContainer::new_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket)
{
    if(m_players.find(a_name) != m_players.end())[[unlikely]]
        return;

    m_players[a_name] = playerPointer(new Player(a_name, a_gender, a_amount, a_client_socket));
    m_action_out.log_in_success(a_name, a_gender, a_client_socket);
    m_action_out.get_player(a_name, a_gender, a_amount);

    for(auto player: m_players)
        m_action_out.get_player(player.second->m_name, player.second->m_gender, player.second->m_vars["m_amount"], a_client_socket);

    if(num_of_players() > 1)
        m_wait.exit_wait();
}

bool PlayersContainer::log_in_chack(std::string& a_name,  int a_client_socket)const
{
    if(m_players.find(a_name) != m_players.end())
    {
        m_action_out.user_name_alredy_log(a_client_socket);
        return false;
    }

    if(num_of_players() == MAX_NUM_OF_PLAYERS)[[unlikely]]
    {
        //implement message ?
        return false;
    }

    return true;
}

void PlayersContainer::delete_player(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end()) [[unlikely]]
        return;

    for(int i = 0; i < 2; ++i)
        deck.push_card(give_card(a_name));
    m_players.erase(a_name);
    m_action_out.delete_player(a_name);
}

void PlayersContainer::delete_player(int a_client_socket)
{
    for(auto player : m_players)
    {
        if(player.second.get()->m_vars["m_socket"] == a_client_socket)
        {
            delete_player(player.second.get()->m_name);
            break;
        }
    }
}

int PlayersContainer::num_of_players()const
{
    return m_players.size();
}

void PlayersContainer::take_chip (std::string& a_name, int a_chip)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name]->m_vars["a_amount"] += a_chip;
}

int PlayersContainer::get (std::string& a_name, std::string& a_var)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return 0;

    return m_players[a_name]->m_vars[a_var];
}

void PlayersContainer::set (std::string& a_name, std::string& a_var, int a_value)
{
    if(m_players.find(a_name) == m_players.end()) [[unlikely]]
        return;

    m_players[a_name]->m_vars[a_var] = a_value;
}

void PlayersContainer::get_card(std::string& a_name, cardPointer a_card)
{
    if(m_players.find(a_name) == m_players.end()) [[unlikely]]
        return;

    m_players[a_name]->m_hand.emplace_back(a_card);
}

cardPointer PlayersContainer::give_card(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end() || m_players[a_name]->m_hand.empty()) [[unlikely]]
        return nullptr;

    int index = m_players[a_name]->m_hand.size()-1;
    cardPointer card = m_players[a_name]->m_hand[index];
    m_players[a_name]->m_hand.pop_back();
    return card;
}

bool PlayersContainer::is_it_has_a_cards(std::string& a_name)
{
    return !m_players[a_name]->m_hand.empty();
}

std::pair<cardPointer,cardPointer> PlayersContainer::give_cards(std::string& a_name)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return std::pair<cardPointer,cardPointer>(nullptr, nullptr);

    std::pair<cardPointer,cardPointer> cards;
    cards.first  =  m_players[a_name]->m_hand[0];
    cards.second =  m_players[a_name]->m_hand[1];
    return cards;
}

void PlayersContainer::turn_on(std::string& a_name, std::string& a_flag)
{
    if(m_players.find(a_name) == m_players.end()
    || m_players[a_name]->m_flags.find(a_flag) == m_players[a_name]->m_flags.end()) [[unlikely]]
        return;

    m_players[a_name]->m_flags[a_flag] = true;
}

void PlayersContainer::turn_off(std::string& a_name, std::string& a_flag)
{
    if(m_players.find(a_name) == m_players.end()
    || m_players[a_name]->m_flags.find(a_flag) == m_players[a_name]->m_flags.end()) [[unlikely]]
        return;

    m_players[a_name]->m_flags[a_flag] = false;
}

bool PlayersContainer::is_flag_on(std::string& a_name, std::string& a_flag)
{
    if(m_players.find(a_name) == m_players.end()
    || m_players[a_name]->m_flags.find(a_flag) == m_players[a_name]->m_flags.end()) [[unlikely]]
        return false;

    return m_players[a_name]->m_flags[a_flag];
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


}// poker namespace
