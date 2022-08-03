#pragma once

#include <mutex>

#include "protocol.hpp"
#include "client_socket.hpp"

namespace poker
{

using Lock = std::unique_lock<std::mutex>;

class TcpClient
{
public: 
    TcpClient(ClientSocket& a_socket);

    bool send_to_server(char* a_buffer, int a_length);
    bool receive_from_server(char* a_buffer);

private:
    std::mutex m_mutex;
    ClientSocket& m_socket;
};


}//namespace poker

