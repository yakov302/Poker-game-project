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

bool TablesContainer::log_in_chack(std::string& a_name, int a_client_socket)
{
    for(auto table : m_tables)
    {
        if(!table.second.get()->m_players.log_in_chack(a_name, a_client_socket))
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout <<__func__ << "(): player(" << a_name << ", " << a_client_socket<< ") alredy in table id: " << table.first << " return false" << std::endl;

            return false;
        }
    }

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): player(" << a_name << ", " << a_client_socket<< ")  not exist in any table" << std::endl;
    
    return true;
}


void TablesContainer::get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket)
{  
    if(m_viewer_socket_to_table_id.find(a_client_socket) != m_viewer_socket_to_table_id.end())
    {  
        int current_table_id = m_viewer_socket_to_table_id[a_client_socket];
        if(!m_tables[current_table_id].get()->is_table_full())
        {
            update_client_screen(m_tables[current_table_id], current_table_id, a_client_socket);
            enter_player_impl(m_tables[current_table_id], current_table_id, a_name, a_gender, a_amount, a_client_socket); 
            return;
        }
        else
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout <<__func__ << "(): call m_action_out.clear_screen(" << a_client_socket<< ") from table id: " <<  current_table_id << std::endl;
            
            m_tables[current_table_id].get()->m_action_out.clear_screen(a_client_socket);

            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout <<__func__ << "(): call m_action_out.table_is_full(" << a_client_socket<< ") from table id: " <<  current_table_id << std::endl;

            m_tables[current_table_id].get()->m_action_out.table_is_full(a_client_socket);
        }
    }
    else
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): "  << a_client_socket << " socket dont exist in m_viewer_socket_to_table_id" << std::endl;
    }

    bool player_enter_table = false;
    while (!player_enter_table)
    { 
        for(auto table : m_tables)
        {
            if(!table.second.get()->is_table_full())
            {
                update_client_screen( table.second,  table.first, a_client_socket);
                enter_player_impl(table.second, table.first, a_name, a_gender, a_amount, a_client_socket);
                player_enter_table = true;
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

void TablesContainer::update_client_screen(tablePointer& table, int table_id, int a_client_socket)
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): call m_action_out.clear_screen(" << a_client_socket<< ") from table id: " <<  table_id << std::endl;

    table.get()->m_action_out.clear_screen(a_client_socket);

    if(table.get()->is_table_empty())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): empty table without players call m_action_out.table_is_empty(" << a_client_socket<< ") from table id: " <<  table_id << std::endl;

        table.get()->m_action_out.table_is_empty(a_client_socket);
    }
    else
    {
        send_to_client_all_table_objects(table, table_id, a_client_socket);

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): call m_tables[" << table_id << "]->m_action_out.clear_text(" << a_client_socket << ")" << std::endl;                
    
        table.get()->m_action_out.clear_text(a_client_socket);
    }
}

void TablesContainer::enter_player_impl(tablePointer& a_table, int table_id, std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket)
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): call table[" << table_id << "]->m_players.new_player(" << a_name << ", " <<  a_gender << ", " << a_amount << ", "  << a_client_socket<< ")" << std::endl;
                
    a_table.get()->m_players.new_player(a_name, a_gender, a_amount, a_client_socket);
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): set m_player_socket_to_table_id[" << a_client_socket << "] = " << table_id << std::endl;

    m_player_socket_to_table_id[a_client_socket] = table_id;

    ++m_num_of_players;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): m_num_of_players increased to " <<  m_num_of_players << std::endl;
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

void TablesContainer::new_table()
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): creates new table. table id: " << m_tables_index << std::endl;
    
    m_tables.emplace(m_tables_index, tablePointer(new TableManager(m_tcp)));
    ++m_tables_index;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): m_tables_index increase to: " << m_tables_index << std::endl;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

int TablesContainer::low_table_id()
{
    int table_id;
    for(auto table : m_tables)
        table_id = table.first;
    return table_id;
}

void TablesContainer::change_table(int a_client_socket)
{   
    int low_table_id_number = low_table_id();
    int current_table_id = m_viewer_socket_to_table_id[a_client_socket];

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): " << current_table_id << " is the current table id" << std::endl;                

    for(auto table : m_tables)
    {  
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): compare " << current_table_id << " current table id to " << table.first << " table id" << std::endl;                

        if(table.first == current_table_id)
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout <<__func__ << "(): same table -> continues" << std::endl;                

            continue;
        }
        
        if((current_table_id != low_table_id_number) && 
           (table.first > current_table_id))
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout <<__func__ << "(): looking lower table id -> continues" << std::endl; 

            continue;
        }

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): set m_viewer_socket_to_table_id[" << a_client_socket << "] = " << table.first << std::endl;                
        
        m_viewer_socket_to_table_id[a_client_socket] = table.first;

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): call m_tables[" << current_table_id << "]->m_action_out.delete_viewer(" << a_client_socket << ")" << std::endl;                

        m_tables[current_table_id].get()->m_action_out.delete_viewer(a_client_socket);

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): call m_tables[" << table.first << "]->m_action_out.get_viewer(" << a_client_socket << ")" << std::endl;                

        table.second.get()->m_action_out.get_viewer(a_client_socket);
        update_client_screen(table.second, table.first, a_client_socket);
        break;
    }

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

void TablesContainer::enter_viewer_impl(tablePointer& a_table, int a_table_id, int a_client_socket)
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): set m_viewer_socket_to_table_id[" << a_client_socket << "] = " << a_table_id << std::endl;                
    
    m_viewer_socket_to_table_id[a_client_socket] = a_table_id;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): call m_action_out.get_viewer(" << a_client_socket<< ") to table id: " <<  a_table_id << std::endl;

    a_table.get()->m_action_out.get_viewer(a_client_socket);

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

void TablesContainer::get_viewer(int a_client_socket)
{
    if(m_tables.empty())
        new_table();

    if(m_player_socket_to_table_id.find(a_client_socket) != m_player_socket_to_table_id.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): socket " << a_client_socket << " found in m_player_socket_to_table_id -> enter by current table id" << std::endl;

        int current_table_id = m_player_socket_to_table_id[a_client_socket];
        enter_viewer_impl(m_tables[current_table_id], current_table_id, a_client_socket);
        update_client_screen(m_tables[current_table_id], current_table_id, a_client_socket);
    }
    else
    {
        for(auto table : m_tables)
        { 
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout <<__func__ << "(): socket " << a_client_socket << " dont found in m_player_socket_to_table_id -> enter by rand table id" << std::endl;

            enter_viewer_impl(table.second, table.first, a_client_socket);
            update_client_screen(table.second, table.first, a_client_socket);
            break;
        }
    }

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

void TablesContainer::delete_viewer(int a_client_socket)
{        
    if(m_viewer_socket_to_table_id.find(a_client_socket) == m_viewer_socket_to_table_id.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): socket " << a_client_socket << " does not exist in m_veiwer_socket_to_table_id map -> return" << std::endl;

        return;
    }

    int table_id = m_viewer_socket_to_table_id[a_client_socket];
    if(m_tables.find(table_id) == m_tables.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " does not exist in m_tables map -> return" << std::endl;
        
        return;
    }

    if(m_player_socket_to_table_id.find(a_client_socket) != m_player_socket_to_table_id.end())
    {
        if(m_player_socket_to_table_id[a_client_socket] != table_id)
        {
            if(dbg[TABLES_CONTAINER])[[unlikely]]
                std::cout << __func__ << "(): call m_tables[" << table_id << "]->m_action_out.delete_viewer(socket = " << a_client_socket << ")" << std::endl;

            m_tables[table_id].get()->m_action_out.delete_viewer(a_client_socket);
        }
    }
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): call m_viewer_socket_to_table_id.erase(" << a_client_socket<< ") to table id: " <<  table_id << std::endl;

    m_viewer_socket_to_table_id.erase(a_client_socket);

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

void TablesContainer::delete_player(int a_client_socket)
{
    if(m_player_socket_to_table_id.find(a_client_socket) == m_player_socket_to_table_id.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): socket " << a_client_socket << " does not exist in m_player_socket_to_table_id map -> return" << std::endl;

        return;
    }

    int table_id = m_player_socket_to_table_id[a_client_socket];
    if(m_tables.find(table_id) == m_tables.end())
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): tables " << table_id << " does not exist in m_tables map -> return" << std::endl;
        
        return;
    }

    delete_player_impl(m_tables[table_id], table_id, a_client_socket);

    find_match_for_singles();    
    check_if_table_need_to_delete(table_id);
}

void TablesContainer::delete_player_impl(tablePointer& a_table, int table_id, int a_client_socket)
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): call m_tables[" << table_id << "]->player_going_to_be_deleted(" << a_client_socket<< ")" << std::endl;

    a_table.get()->player_going_to_be_deleted(a_client_socket);
    
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_tables[" << table_id << "]->m_players.delete_player(" << a_client_socket << ")" << std::endl;
    
    a_table.get()->m_players.delete_player(a_client_socket);

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call m_tables[" << table_id << "]->player_deleted()" << std::endl;
    
    a_table.get()->player_deleted();

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): call m_tables[" << table_id << "]->m_player_socket_to_table_id.erase(" << a_client_socket<< ")" << std::endl;

    m_player_socket_to_table_id.erase(a_client_socket);
    --m_num_of_players;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout <<__func__ << "(): m_num_of_players decreased to " <<  m_num_of_players << std::endl;

    if(dbg[TABLES_CONTAINER])[[unlikely]]
        print_table_container();
}

int TablesContainer::num_of_viewers_in_table(int table_id)
{
    int num_of_viewers = 0;

    for(auto viewer : m_viewer_socket_to_table_id)
    {
        if(viewer.second == table_id)
            ++num_of_viewers;
    }

    return num_of_viewers;
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
        
        std::cout << std::endl << " " << num_of_viewers_in_table(table.first) << " viewer: " << std::endl;
        for(auto viewer : m_viewer_socket_to_table_id)
        {
            if(viewer.second == table.first)
                std::cout << " - " << viewer.first << std::endl;
        }

        std::cout << std::endl << " " << table.second.get()->m_action_out.m_sockets.size() << " action out sockets: " << std::endl;
        for(auto socket : table.second.get()->m_action_out.m_sockets)
            std::cout << " - " << socket << std::endl;

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
            std::cout << __func__ << "(): tables " << table_id << " is empty!" << std::endl;

        for(auto dst_table : m_tables)
        {
            if(dst_table.first == table_id)
                continue;

            if(!dst_table.second.get()->is_table_empty())
            {
                move_all_viewers_to_another_table(dst_table.second, m_tables[table_id], dst_table.first);
                break;
            }
        }

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): call m_tables.erase(" << table_id << ")" << std::endl;

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

    delete_player_impl(srs_table.second,  srs_table.first, player.get()->m_vars[socket]);
  
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): call table[" << dst_table.first << "]->m_action_out.clear_screen(" << player.get()->m_vars[socket] << ")" << std::endl;

    dst_table.second.get()->m_action_out.clear_screen(player.get()->m_vars[socket]);
    send_to_client_all_table_objects(dst_table.second, dst_table.first, player.get()->m_vars[socket]);

    enter_player_impl(dst_table.second, dst_table.first, player.get()->m_name, player.get()->m_gender, player.get()->m_vars[amount], player.get()->m_vars[socket]);
    
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

void TablesContainer::send_to_client_all_table_objects(tablePointer& a_table, int table_id, int a_client_socket)
{
    if(dbg[TABLES_CONTAINER])[[unlikely]]
        std::cout << __func__ << "(): send to client : " << a_client_socket << " all table " << table_id << " objects" << std::endl;
    
    a_table.get()->m_players.send_to_new_player_all_existing_players(a_client_socket);
    a_table.get()->m_table.send_new_player_existing_chips_and_cards(a_client_socket);
}

void TablesContainer::move_all_viewers_to_another_table(tablePointer& dst_table, tablePointer& srs_table, int dst_table_id)
{
    for(auto viewer : m_viewer_socket_to_table_id)
    {
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): move viewer from table " << m_viewer_socket_to_table_id[viewer.first] << " to table " << dst_table_id << std::endl;
        
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): set m_viewer_socket_to_table_id["<< viewer.first << "] = " << dst_table_id << std::endl;
        
        m_viewer_socket_to_table_id[viewer.first] = dst_table_id;

        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): srs_table.get()->m_action_out.delete_viewer(" << viewer.first << ")" << std::endl;
        
        srs_table.get()->m_action_out.delete_viewer(viewer.first);
        
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout <<__func__ << "(): call dst_table.get()->m_action_out.get_viewer(" << viewer.first << ")" << std::endl;

        dst_table.get()->m_action_out.get_viewer(viewer.first);
        
        if(dbg[TABLES_CONTAINER])[[unlikely]]
            std::cout << __func__ << "(): call m_action_out.clear_screen(" << viewer.first << ")" << std::endl;

        dst_table.get()->m_action_out.clear_screen(viewer.first);
        send_to_client_all_table_objects(dst_table, dst_table_id, viewer.first);
    }
}


}// poker namespace
