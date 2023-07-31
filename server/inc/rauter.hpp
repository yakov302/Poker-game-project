#pragma once

#include <thread>

#include "cli_cmd.hpp"
#include "action_in.hpp"
#include "tcp_server.hpp"
#include "msg_memory.hpp"
#include "server_socket.hpp"
#include "tables_container.hpp"

#define MAX_SOCKET_NUM_TO_LISTEN 1024

namespace poker
{

class Rauter
{
public: 
    Rauter(ServerSocket& a_socket, TcpServer& a_tcp, ActionIn& a_action, MsgMemory& a_msg_memory);
    ~Rauter();

    void run();
    void stop();

private: 
    void delete_deleted(); 
    void accept_new_client();
    void take_care_exists_clients();
    void fatal_error(std::string a_fail);

private:
    bool m_stop;
    int m_activity;
    std::thread* m_thread;

    TcpServer& m_tcp;
    ActionIn& m_action_in;
    ServerSocket& m_socket;
    MsgMemory& m_msg_memory;
};


}//namespace poker
