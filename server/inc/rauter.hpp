#pragma once

#include <iostream>
#include <vector>
#include <thread>

#include "server_socket.hpp"
#include "protocol.hpp"
#include "tcp_server.hpp"
#include "action_in.hpp"
#include "players_container.hpp"

#define BUFFER_SIZE 2048

namespace poker
{

class Rauter
{
public: 
    Rauter(ServerSocket& a_socket, TcpServer& a_tcp, ActionIn& a_action, PlayersContainer& a_players);
    Rauter(Rauter const& a_source) = delete;
    Rauter& operator=(Rauter const& a_source) = delete;
    ~Rauter();

    void run();
    void stop();

private: 
    void delete_deleted(); 
    void accept_new_client();
    void take_care_exists_clients();
    void fatal_error(std::string a_fail);

private:
    bool m_stop;
    char* m_buffer;
    int m_activity;
    std::thread* m_thread;

    ServerSocket& m_socket;
    TcpServer& m_tcp;
    ActionIn& m_action_in;
    PlayersContainer& m_players;
};


}//namespace poker
