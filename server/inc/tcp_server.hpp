#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>

#include "protocol.hpp"

namespace poker
{

class TcpServer
{
public: 
    TcpServer(std::string a_server_ip, int a_servr_port);
    TcpServer(TcpServer const& a_source) = delete;
    TcpServer& operator=(TcpServer const& a_source) = delete;
  
    int accept_new_client(struct sockaddr_in& a_sin, int a_listen_socket);
    bool receive_from_client(int a_client_socket, char* a_buffer);
    bool send_to_client(int a_client_socket, char* a_buffer, int a_message_size);
    void send_all_clients(char* a_buffer, int a_message_size);
    void delete_client(int a_client_socket, std::vector<int>::iterator& a_it);
    void close_all_clients_sockets();
    void fatal_error(std::string a_fail);

    int& listen_socket();
    int& num_of_clients();
    fd_set& source_sockets_bits();
    std::vector<int>& clients_sockets();
    std::vector<int>& deleted_sockets();

private:
    int m_listen_socket;
    int m_num_of_clients;
    struct sockaddr_in m_sin;
    fd_set m_source_sockets_bits;
    std::vector<int> m_clients_sockets;
    std::vector<int> m_deleted_sockets;
};


}//namespace poker
