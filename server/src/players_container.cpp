#include "players_container.hpp"

namespace poker
{

std::string bet = "bet";
std::string fold = "fold";
std::string viewer = "viewer";
std::string socket = "socket";
std::string amount = "amount";
std::string result = "result";
std::string my_turn = "my_turn";
extern bool dbg[NUM_OF_DBG_TYPES];

PlayersContainer::PlayersContainer(ActionOut& a_action_out, Deck& a_deck)
: m_wait()
, m_deck(a_deck)
, m_action_out(a_action_out)
, m_players()
{
    m_players.reserve(MAX_NUM_OF_PLAYERS_IN_TABLE);
}

void PlayersContainer::new_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket)
{
    if(m_players.find(a_name) != m_players.end())[[unlikely]]
    {
        if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): player " << a_name << " already exists in the table -> return" << std::endl;
        
        return;
    }

    if(dbg[PLAYERS_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): set m_players[" << a_name << "] = playerPointer(new Player(" << a_name << ", " << a_gender << ", " << a_amount << ", " << a_client_socket << ")" << std::endl;

    m_players[a_name] = playerPointer(new Player(a_name, a_gender, a_amount, a_client_socket));
    m_action_out.play_success(a_client_socket);
    m_action_out.get_player(a_name, a_gender, a_amount);

    if(num_of_players() > 1)
        m_wait.exit_wait();
}

void PlayersContainer::send_to_new_player_all_existing_players(int a_client_socket)
{
    for(auto player: m_players)
    {
        if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): call m_action_out.get_player(" << player.second->m_name << ", " << player.second->m_gender << ", " << player.second->m_vars[amount] << ", " << a_client_socket << ")" << std::endl;

        m_action_out.get_player(player.second->m_name, player.second->m_gender, player.second->m_vars[amount], a_client_socket);
        if(!player.second->m_hand.empty())
        {
            for(auto card : player.second->m_hand)
            {
                if(dbg[PLAYERS_CONTAINER])[[unlikely]]
                    std::cout << __func__ << "(): call m_action_out.get_card(" << player.second->m_name << ", " << card << ", " << a_client_socket << ")" << std::endl;
                
                m_action_out.get_card(player.second->m_name, card, a_client_socket);
            }
            
            if(player.second->m_flags[my_turn])
            {
                if(dbg[PLAYERS_CONTAINER])[[unlikely]]
                    std::cout << __func__ << "(): call m_action_out.turn_on(" << player.second->m_name << ", my_turn" << ")" << std::endl;

                m_action_out.turn_on(player.second->m_name, my_turn);
            }
        }
    }
}

bool PlayersContainer::log_in_chack(std::string& a_name,  int a_client_socket)const
{
    if(m_players.find(a_name) != m_players.end())
    {
        if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): player " << a_name << " already exists in the table -> return false" << std::endl;
        
        m_action_out.user_name_alredy_log(a_client_socket);
        return false;
    }

    if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): player " << a_name << " does not exist in the table -> return true" << std::endl;

    return true;
}

void PlayersContainer::delete_player(std::string& a_name, int a_client_socket)
{
    if(m_players.find(a_name) == m_players.end()) [[unlikely]]
    {
        if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): player " << a_name << " does not exist in the table -> cant delete player" << std::endl;
        
        return;
    }

    if(dbg[PLAYERS_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_players.erase(" << a_name << ")" << std::endl;

    for(int i = 0; i < 2; ++i)
        m_deck.push_card(give_card(a_name));

    m_players.erase(a_name);
    m_action_out.delete_player(a_name, a_client_socket);
}

void PlayersContainer::delete_player(int a_client_socket)
{
    for(auto player : m_players)
    {
        if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): comper " <<  player.second.get()->m_vars[socket] << " to " << a_client_socket << std::endl;
        
        if(player.second.get()->m_vars[socket] == a_client_socket)
        {
            if(dbg[PLAYERS_CONTAINER])[[unlikely]]
                std::cout << __func__ << "(): call delete_player(name: " << player.second.get()->m_name << ", client_socket: " << a_client_socket << ")" << std::endl;
            
            delete_player(player.second.get()->m_name, a_client_socket);
            break;
        }
    }
}

std::string PlayersContainer::name(int a_client_socket)
{
    for(auto player : m_players)
    {
        if(dbg[PLAYERS_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): comper " <<  player.second.get()->m_vars[socket] << " to " << a_client_socket << std::endl;
        
        if(player.second.get()->m_vars[socket] == a_client_socket)
            return player.second.get()->m_name;
    }

    return "";
}

int PlayersContainer::num_of_players()const
{
    return m_players.size();
}

void PlayersContainer::take_chip (std::string& a_name, int a_chip)
{
    if(m_players.find(a_name) == m_players.end())[[unlikely]]
        return;

    m_players[a_name]->m_vars[amount] += a_chip;
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

std::pair<cardPointer,cardPointer> PlayersContainer::show_cards(std::string& a_name)
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

playerPointer& PlayersContainer::give_lest_player()
{
    return m_players.begin()->second;
}

void PlayersContainer::print_players()
{
    auto it = m_players.begin();
    auto end = m_players.end();

    std::cout << std::endl <<  "----------------print_players----------------" << std::endl;
    while(it != end)
    {
        std::cout << " - name: " << (*it).second.get()->m_name << ", socket: " << (*it).second.get()->m_vars[socket] << ", is viewer: ";
        if((*it).second.get()->m_vars[viewer])
            std::cout << "true" << std::endl;
        else
            std::cout << "false" << std::endl;
        it++;
    }
    std::cout << "---------------------------------------------" << std::endl << std::endl;
}


}// poker namespace
