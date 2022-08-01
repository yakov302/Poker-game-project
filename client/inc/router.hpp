#pragma once

#include <unordered_map>
#include <thread>

#include "tcp_client.hpp"
#include "protocol.hpp"
#include "action_in.hpp"
#include "table.hpp"
#include "client_socket.hpp"

#define BUFFER_SIZE 2048

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

    ClientSocket& m_socket;
    TcpClient& m_tcp;
    ActionIn& m_action_in;
    Table& m_table;
};


}// poker namespace
