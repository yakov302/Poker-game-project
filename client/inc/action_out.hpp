#pragma once

#include "tcp_client.hpp"
#include "protocol.hpp"

namespace poker
{

class ActionOut
{
public:
    ActionOut(TcpClient& a_tcp);
    ~ActionOut();

    void registration_request(std::string a_name, std::string a_password, std::string a_gender);
    void log_in_request(std::string a_name, std::string a_password, int a_amount);
    void bet_action(std::string a_name, int a_chip);
    void finish_bet(std::string a_name);
    void check_action(std::string a_name);
    void fold_action(std::string a_name);
    void wake_up(int a_socket);

private:
    void name_and_message(std::string a_name, Message_type a_message);

private:
    char* m_buffer;
    TcpClient& m_tcp;
};


}// poker namespace
