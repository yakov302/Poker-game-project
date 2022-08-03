#pragma once

#include <thread>

#include "table.hpp"
#include "action_in.hpp"
#include "tcp_client.hpp"
#include "client_socket.hpp"

#define BUFFER_SIZE 256

namespace poker
{

class Router 
{
public:
    Router(ClientSocket& a_socket, ActionIn& a_action, TcpClient& a_tcp, Table& a_table);
    ~Router();

    void run();

private:
    char* m_buffer;
    std::thread* m_thread;

    Table& m_table;
    TcpClient& m_tcp;
    ActionIn& m_action_in;
    ClientSocket& m_socket;
};


}// poker namespace
