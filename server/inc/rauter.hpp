#pragma once

#include <thread>

#include "tcp_server.hpp"
#include "action_in.hpp"
#include "server_socket.hpp"
#include "players_container.hpp"

#define BUFFER_SIZE 512
#define MAX_SOCKET_NUM_TO_LISTEN 1024

namespace poker
{

class Rauter
{
public: 
    Rauter(ServerSocket& a_socket, TcpServer& a_tcp, ActionIn& a_action, PlayersContainer& a_players);
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

    TcpServer& m_tcp;
    ActionIn& m_action_in;
    ServerSocket& m_socket;
    PlayersContainer& m_players;
};


}//namespace poker
