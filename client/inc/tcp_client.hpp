#pragma once

#include "client_socket.hpp"
#include "protocol.hpp"


namespace poker
{

class TcpClient
{
public: 
    TcpClient(ClientSocket& a_socket);

    bool send_to_server(char* a_buffer, int a_length);
    bool receive_from_server(char* a_buffer);

private:
   ClientSocket& m_socket;
};


}//namespace poker

