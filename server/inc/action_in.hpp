#pragma once

#include "cli_cmd.hpp"
#include "protocol.hpp"
#include "subscribs.hpp"
#include "action_out.hpp"
#include "tables_container.hpp"

namespace poker
{

class ActionIn
{
public:
    ActionIn(TcpServer& a_tcp, Subscribs& a_subscribs, TablesContainer& a_tables_container);

    void get(char* a_buffer, int a_client_socket);
    void player_deleted(int a_client_socket);

private:
    void registration_reques(char* a_buffer, int a_client_socket);
    void log_in_reques(char* a_buffer, int a_client_socket);

    void finish_bet(int a_client_socket);
    void start_bet(int a_client_socket);
    void bet(char* a_buffer, int a_client_socket);

    void fold(int a_client_socket);
    void check(int a_client_socket);
    void wake_up_client(int a_client_socket);

private:
    ActionOut m_action_out; 
    Subscribs& m_subscribs;
    TablesContainer& m_tables_container;
};


}// poker namespace
