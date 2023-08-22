#pragma once

#include "cli_cmd.hpp"
#include "table_manager.hpp"

namespace poker
{

using tablePointer = std::shared_ptr<TableManager>;
using tableIdAndPtrPair = std::pair<const int, poker::tablePointer>;

class TablesContainer
{
public:
    TablesContainer(TcpServer& a_tcp);

    void get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket);
    bool log_in_chack(std::string& a_name, int a_client_socket);
    void delete_player(int a_client_socket);
    void delete_viewer(int a_client_socket);
    void get_viewer(int a_client_socket);
    void stop();

    int num_of_players();
    bool is_table_is_alive(int a_table_id);

private:
    void new_table();
    void print_table_container();
    void find_match_for_singles();
    void check_if_table_need_to_delete(int table_id);
    void delete_player_impl(tablePointer& a_table, int table_id, int a_client_socket);
    void send_to_new_client_all_table_objects(tablePointer& a_table, int a_client_socket);
    void move_player_to_another_table(tableIdAndPtrPair& dst_table, tableIdAndPtrPair& srs_table);
    void move_all_viewers_to_another_table(tablePointer& dst_table, tablePointer& srs_table, int dst_table_id);

private:
    friend class ActionIn;

    int m_tables_index;
    int m_num_of_players;

    TcpServer& m_tcp;
    std::unordered_map<int, tablePointer> m_tables;
    std::unordered_map<int, int> m_socket_to_table_id;
};


}// poker namespace
