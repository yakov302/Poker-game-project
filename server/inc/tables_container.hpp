#pragma once

#include "cli_cmd.hpp"
#include "table_manager.hpp"

namespace poker
{

using tablePointer = std::shared_ptr<TableManager>;

class TablesContainer
{
public:
    TablesContainer(TcpServer& a_tcp);

    void get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket);
    void delete_player(int a_client_socket);
    void stop();

private:
    void new_table();

private:
    friend class ActionIn;

    int m_tables_index;
    TcpServer& m_tcp;
    std::unordered_map<int, tablePointer> m_tables;
    std::unordered_map<int, int> m_socket_to_table_id;
};


}// poker namespace
