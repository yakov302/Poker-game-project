#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include "protocol.hpp"

namespace poker
{

class TcpClient
{
public: 
    TcpClient(std::string a_server_ip, int a_servr_port);
    ~TcpClient();

    void fatal_error(std::string a_fail);
    bool send_to_server(char* a_buffer, int a_length);
    bool receive_from_server(char* a_buffer);
    bool error(std::string a_fail);
    int socket();

private:
    int m_socket;
    struct sockaddr_in m_sin;
};


}//namespace poker

