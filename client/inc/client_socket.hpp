#pragma once

#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

namespace poker
{

class ClientSocket
{
public:
    ClientSocket(std::string a_server_ip, int a_server_port);
    ~ClientSocket();

    int& socket();
    fd_set& fd();

private:
    fd_set m_fd;
    int m_socket;
    struct sockaddr_in m_sin;
};


}// poker namespace
