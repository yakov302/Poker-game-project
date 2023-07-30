#pragma once

#include "cli_cmd.hpp"
#include "protocol.hpp"
#include "subscribs.hpp"
#include "action_out.hpp"
#include "msg_memory.hpp"
#include "tables_container.hpp"

#define NUM_OF_MESSAGES_IN_MEMORY 128
#define NUM_OF_PLAYERS_PER_THREADS 32

namespace poker
{

class ActionIn
{
public:
    ActionIn(TcpServer& a_tcp, Subscribs& a_subscribs, TablesContainer& a_tables_container, MsgMemory& a_msg_memory);
    ~ActionIn();

    void stop();
    void run(int a_thread_id);
    void player_deleted(int a_client_socket);

private:
    void get(char* a_buffer, int a_client_socket);

    void registration_reques(char* a_buffer, int a_client_socket);
    void log_in_reques(char* a_buffer, int a_client_socket);

    void finish_bet(int a_client_socket);
    void start_bet(int a_client_socket);
    void bet(char* a_buffer, int a_client_socket);

    void fold(int a_client_socket);
    void check(int a_client_socket);
    void wake_up_client(int a_client_socket);

    void open_thread();
    void delete_thread();
    void check_thread_allocation();

private:
    std::vector<bool> m_stop;
    std::vector<std::thread*> m_threads;

    ActionOut m_action_out;
    Subscribs& m_subscribs;
    MsgMemory& m_msg_memory; 
    TablesContainer& m_tables_container;
};


struct Arg
{
    poker::ActionIn* m_action_in;
    size_t m_thread_id;
};


}// poker namespace
