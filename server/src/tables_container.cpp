#include "tables_container.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

TablesContainer::TablesContainer(TcpServer& a_tcp)
: m_tables_index(0)
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
                    std::cout <<__func__ << "(): player alredy in table id" << table.first << std::endl;
                break;
            }

            if(!table.second.get()->is_table_full())
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout <<__func__ << "(): send m_players.new_player(" << a_name << ", " <<  a_gender << ", " << a_amount << ", "  << a_client_socket<< ") to table id: " <<  table.first << std::endl;
            
                table.second.get()->m_players.new_player(a_name, a_gender, a_amount, a_client_socket);
                m_socket_to_table_id[a_client_socket] = table.first;
                player_enter_table = true;
                break;
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
    int table_id = m_socket_to_table_id[a_client_socket];
    std::cout << __func__ << "(): m_tables[" << table_id << "]->m_players.delete_player(a_client_socket = " << a_client_socket << ")" << std::endl;
    m_tables[table_id].get()->m_players.delete_player(a_client_socket); 
}

void TablesContainer::stop()
{
    for(auto table : m_tables)
        table.second.get()->m_game.stop();
}


}// poker namespace
