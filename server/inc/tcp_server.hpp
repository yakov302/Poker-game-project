#pragma once

#include <mutex>

#include "server_socket.hpp"

namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class TcpServer
{
public: 
    TcpServer(ServerSocket& a_socket);
    TcpServer(TcpServer const& a_source) = delete;
    TcpServer& operator=(TcpServer const& a_source) = delete;
  
    int accept_new_client();
    bool receive_from_client(int a_client_socket, char* a_buffer);
    bool send_to_client(int a_client_socket, char* a_buffer, int a_message_size);
    void send_all_clients(char* a_buffer, int a_message_size);

private:
    std::mutex m_mutex;
    ServerSocket& m_socket;   
};


}//namespace poker
