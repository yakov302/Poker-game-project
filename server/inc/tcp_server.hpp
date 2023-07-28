#pragma once

#include <mutex>

#include "server_socket.hpp"
#include "tcp_server.hpp"
#include "cli_cmd.hpp"

namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class TcpServer
{
public: 
    TcpServer(ServerSocket& a_socket);
    
    int accept_new_client();
    bool receive_from_client(int a_client_socket, char* a_buffer);
    bool send_to_client(int a_client_socket, char* a_buffer, int a_message_size);
    void send_all_clients(char* a_buffer, int a_message_size, std::list<int>& a_sockts);

private:
    std::mutex m_mutex;
    ServerSocket& m_socket;   
};


}//namespace poker
