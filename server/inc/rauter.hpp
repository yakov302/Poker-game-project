#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <thread>

#include "protocol.hpp"
#include "tcp_server.hpp"
#include "action_in.hpp"
#include "players_container.hpp"


namespace poker
{

class Rauter
{
public: 
    Rauter(TcpServer& a_tcp, ActionIn& a_action, PlayersContainer& a_players);
    Rauter(Rauter const& a_source) = delete;
    Rauter& operator=(Rauter const& a_source) = delete;
    ~Rauter();

    void run();
    void stop();

private: 
    void delete_deleted(); 
    void accept_new_client();
    void take_care_exists_clients();
    void move_client_to_back(int a_client_socket, std::vector<int>::iterator& a_it);
    void insert_client(int a_client_socket);
    void fatal_error(std::string a_fail);


private:
    bool m_stop;
    char* m_buffer;
    int m_activity;
    std::thread* m_thread;
    fd_set m_temp_sockets_bits;
    struct sockaddr_in m_sin;

    fd_set& m_source_sockets_bits;
    int& m_listen_socket;
    int& m_num_of_clients;
    std::vector<int>& m_clients_sockets;
    std::vector<int>& m_deleted_sockets;

    TcpServer& m_tcp;
    ActionIn& m_action_in;
    PlayersContainer& m_players;
};


}//namespace poker
