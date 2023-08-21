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

    void registration_request(const std::string& a_name, const std::string& a_password, const std::string& a_gender);
    void log_in_request(const std::string& a_name, const std::string& a_password);
    void play_request(const std::string& a_name, int a_amount);
    void view_request();

    void start_bet(const std::string& a_name);
    void bet_action(const std::string& a_name, int a_chip);
    void finish_bet(const std::string& a_name);

    void check_action();
    void fold_action();

    void wake_up_client();
    void wake_up_server();

private:
    void name_and_message(const std::string& a_name, Message_type a_message);
    void pack_and_send(Args& a_arg, Message_type a_message);
    void just_message(Message_type a_message);

private:
    TcpClient& m_tcp;
};


}// poker namespace
