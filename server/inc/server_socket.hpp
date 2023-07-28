#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <string>
#include <list>

#include "cli_cmd.hpp"

namespace poker
{

class ServerSocket
{
public:
    ServerSocket();
    ~ServerSocket();

    int& num_of_clients();
    int& listen_socket();

    struct sockaddr_in& client_sin();
    bool is_in_active_fd(int a_socket);

    fd_set& copy_of_source_fd();
    std::list<int>& deleted_sockets();
    std::list<int>& connected_sockets();

    void delete_less_active_client();
    void insert_client(int a_client_socket);
    void delete_client_by_socket(int a_socket);
    void delete_client_by_iterator(std::list<int>::iterator& a_it);
    void move_client_to_front(std::list<int>::iterator& a_it);
    void delete_from_deleted_sockets(std::list<int>::iterator& a_it);

    void close_all_clients_sockets();
    void fatal_error(std::string a_fail);

private:
    int m_listen_socket;
    int m_num_of_clients;

    fd_set m_source_fd;
    fd_set m_active_fd;

    struct sockaddr_in m_server_sin;
    struct sockaddr_in m_client_sin;
    
    std::list<int> m_connected_sockets;
    std::list<int> m_deleted_sockets;
};


}// poker namespace
