#pragma once

#include <unordered_map>
#include <thread>

#include "tcp_client.hpp"
#include "protocol.hpp"
#include "action_in.hpp"
#include "table.hpp"

namespace poker
{

class Router 
{
public:
    Router(ActionIn& a_action, TcpClient& a_tcp, Table& a_table);
    ~Router();

    void run();

private:
    char* m_buffer;
    std::thread* m_thread;
    fd_set m_fd;

    TcpClient& m_tcp;
    ActionIn& m_action_in;
    Table& m_table;
};


}// poker namespace
