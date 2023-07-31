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

                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    print_table_container();

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

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
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
    if(m_tables.find(table_id) == m_tables.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " does not exist in m_tables map -> return" << std::endl;
        
        return;
    }

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_tables[" << table_id << "]->m_players.delete_player(socket = " << a_client_socket << ")" << std::endl;
    
    m_tables[table_id].get()->m_game.player_going_to_be_deleted(a_client_socket);
    m_tables[table_id].get()->m_bet_round.player_going_to_be_deleted(a_client_socket);
    m_tables[table_id].get()->m_players.delete_player(a_client_socket);
    m_tables[table_id].get()->m_bet_round.player_deleted();
    m_socket_to_table_id.erase(a_client_socket);
    --m_num_of_players;

    if(dbg[TABLES_CONTAINER])[[unlikely]]  
        print_table_container();

    find_match_for_singles();    
    check_if_table_need_to_delete(table_id);
}

void TablesContainer::print_table_container()
{
    std::cout << std::endl <<  "----------------print_table_container----------------" << std::endl;
    for(auto table : m_tables)
    {
        std::cout << std::endl << "------table id " << table.first << "------" << std::endl;
        std::cout << " " << table.second.get()->m_players.num_of_players() << " players: " << std::endl;
        for(auto player : table.second.get()->m_players)
            std::cout << " - " << "name: " << player.first << ", " << "socket: " <<  player.second.get()->m_vars[socket] << std::endl;
        std::cout << "----------------------" << std::endl;
    }
    std::cout << std::endl <<  "-----------------------------------------------------" << std::endl;
}

void TablesContainer::check_if_table_need_to_delete(int table_id)
{
    if(m_tables.find(table_id) == m_tables.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " does not exist in m_tables map ->return" << std::endl;
        
        return;
    }

    if(m_tables[table_id].get()->is_table_empty() && m_tables.size() > 1)
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " is empty call m_tables.erase(" << table_id << ")" << std::endl;

        m_tables[table_id].get()->m_game.stop();
        m_tables.erase(table_id);

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            print_table_container();
    }
    else   
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " not empty or empty but the last table -> not deleted" << std::endl;
    }
}


void TablesContainer::move_player_to_another_table(tableIdAndPtrPair& dst_table, tableIdAndPtrPair& srs_table)
{
    auto player = srs_table.second.get()->m_players.give_lest_player();
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_tables[" << dst_table.first << "]->m_players.new_player(" << player.get()->m_name << ", " << player.get()->m_gender << ", " <<  player.get()->m_vars[amount] << ", " << player.get()->m_vars[socket] << ")" << std::endl;

    dst_table.second.get()->m_players.new_player(player.get()->m_name, player.get()->m_gender, player.get()->m_vars[amount], player.get()->m_vars[socket]);
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_tables[" << srs_table.first << "]->m_players.delete_player(" << player.get()->m_vars[socket] << ")" << std::endl;

    srs_table.second.get()->m_players.delete_player(player.get()->m_vars[socket]);
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): set m_socket_to_table_id["<< player.get()->m_vars[socket] << "] = " << dst_table.first << std::endl;

    m_socket_to_table_id[player.get()->m_vars[socket]] = dst_table.first;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
    
    check_if_table_need_to_delete(srs_table.first);
}

void TablesContainer::find_match_for_singles()
{
    if(m_tables.size() <= 1)
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): no more then 1 table -> no work to do -> return" << std::endl;
            
        return;
    }

    bool match = false;
    for(auto srs_table : m_tables)
    {
        if(srs_table.second.get()->m_players.num_of_players() > 1)
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout << __func__ << "(): in table " << srs_table.first << " no single player continue to next table" << std::endl;

            continue;
        }

        for(auto dst_table : m_tables)
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout << __func__ << "(): in table " << srs_table.first << " there is a single player looking for empty seat at the table " << dst_table.first << std::endl;
            
            if(srs_table.first == dst_table.first)
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout << __func__ << "(): same table, continues to the next table" << std::endl;
                
                continue;
            }
            
            if(dst_table.second.get()->m_players.num_of_players() < MAX_NUM_OF_PLAYERS_IN_TABLE)
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout << __func__ << "(): find empty seat at the table " << dst_table.first << std::endl;

                move_player_to_another_table(dst_table, srs_table);
                match = true;
                break;
            }
            else
            {
                if(dbg[TABLES_CONTAINER])[[unlikely]]
                    std::cout << __func__ << "(): table " << dst_table.first <<  " is full cant accept new player" << std::endl;
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
