#pragma once

#include "tcp_client.hpp"
#include "protocol.hpp"

#define BUFFER_SIZE 256

namespace poker
{

class ActionOut
{
public:
    ActionOut(TcpClient& a_tcp);

    void registration_request(std::string a_name, std::string a_password, std::string a_gender);
    void log_in_request(std::string a_name, std::string a_password, int a_amount);

    void start_bet(std::string a_name);
    void bet_action(std::string a_name, int a_chip);
    void finish_bet(std::string a_name);

    void check_action();
    void fold_action();

    void wake_up_client();
    void wake_up_server();

private:
    void name_and_message(std::string a_name, Message_type a_message);
    void pack_and_send(Args& a_arg, Message_type a_message);
    void just_message(Message_type a_message);

private:
    TcpClient& m_tcp;
};


}// poker namespace
