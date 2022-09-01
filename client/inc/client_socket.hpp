#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <string>

namespace poker
{

class ClientSocket
{
public:
    ClientSocket();
    ~ClientSocket();

    int& socket();
    fd_set& fd();

private:
    fd_set m_fd;
    int m_socket;
    struct sockaddr_in m_sin;
};


}// poker namespace
