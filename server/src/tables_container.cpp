#include "tables_container.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];
extern std::string socket;
extern std::string amount;

TablesContainer::TablesContainer(TcpServer& a_tcp)
: m_tables_index(0)
, m_num_of_players(0)
, m_tcp(a_tcp)
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__<< "(): constructor was built" << std::endl;
}

void TablesContainer::get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket)
{
    bool player_enter_table = false;
    
    while (!player_enter_table)
    { 
        for(auto table : m_tables)
        {
            if(!table.second.get()->m_players.log_in_chack(a_name, a_client_socket))
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout <<__func__ << "(): player(" << a_name << ", " <<  a_gender << ", " << a_amount << ", "  << a_client_socket<< ") alredy in table id: " << table.first << std::endl;
                player_enter_table = true;
                break;
            }

            if(!table.second.get()->is_table_full())
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout <<__func__ << "(): call m_players.new_player(" << a_name << ", " <<  a_gender << ", " << a_amount << ", "  << a_client_socket<< ") to table id: " <<  table.first << std::endl;
            
                table.second.get()->m_players.new_player(a_name, a_gender, a_amount, a_client_socket);
                m_socket_to_table_id[a_client_socket] = table.first;
                player_enter_table = true;
                ++m_num_of_players;
                break;
            }
            else
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout <<__func__ << "(): table id " << table.first << " is full" << std::endl;
            }
        }
        
        if(!player_enter_table)
            new_table();
    }
}

void TablesContainer::new_table()
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): creates new table. table id: " << m_tables_index << std::endl;
    
    m_tables.emplace(m_tables_index, tablePointer(new TableManager(m_tcp)));
    ++m_tables_index;
}

void TablesContainer::delete_player(int a_client_socket)
{
    if(m_socket_to_table_id.find(a_client_socket) == m_socket_to_table_id.end())[[unlikely]]
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): socket " << a_client_socket << " does not exist in m_socket_to_table_id map -> client not log in yet, no need to delete." << std::endl;

        return;
    }

    int table_id = m_socket_to_table_id[a_client_socket];

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_tables[" << table_id << "]->m_players.delete_player(a_client_socket = " << a_client_socket << ")" << std::endl;
    
    m_tables[table_id].get()->m_players.delete_player(a_client_socket);
    --m_num_of_players;

    find_match_for_singles();    
    check_if_table_need_to_delete(table_id);
}

void TablesContainer::check_if_table_need_to_delete(int table_id)
{
    if(m_tables[table_id].get()->is_table_empty() && m_tables.size() > 1)
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " is empty call m_tables.erase(" << table_id << ")" << std::endl;

        m_tables[table_id].get()->m_game.stop();
        m_tables.erase(table_id);
    }
}

void TablesContainer::find_match_for_singles()
{
    if(m_tables.size() <= 1)
        return;

    bool match = false;
    for(auto first_table : m_tables)
    {
        if(first_table.second.get()->m_players.num_of_players() > 1)
            continue;

        for(auto second_table : m_tables)
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout << __func__ << "(): first_table: " << first_table.first << " second_table: " << second_table.first << std::endl;
            
            if(first_table.first == second_table.first)
                continue;
            
            if(second_table.second.get()->m_players.num_of_players() < MAX_NUM_OF_PLAYERS_IN_TABLE)
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout << __func__ << "(): move lest player from tables " << first_table.first << " to table " << second_table.first << std::endl;

                auto player = first_table.second.get()->m_players.give_lest_player();
                second_table.second.get()->m_players.new_player(player.get()->m_name, player.get()->m_gender, player.get()->m_vars[amount], player.get()->m_vars[socket]);
                first_table.second.get()->m_players.delete_player(player.get()->m_vars[socket]);
                m_socket_to_table_id[player.get()->m_vars[socket]] = second_table.first;
                check_if_table_need_to_delete(first_table.first);
                match = true;
                break;
            }
        }

        if(match){break;}
    }
}

void TablesContainer::stop()
{
    for(auto table : m_tables)
        table.second.get()->m_game.stop();
}

bool TablesContainer::is_table_is_alive(int a_table_id)
{
    if(m_tables.find(a_table_id) == m_tables.end())
        return false;
    return true;
}

int TablesContainer::num_of_players()
{
    return m_num_of_players;
}


}// poker namespace
