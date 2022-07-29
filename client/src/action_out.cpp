#include "action_out.hpp"
#include <iostream>

namespace poker
{

ActionOut::ActionOut(TcpClient& a_tcp)
: m_tcp(a_tcp)
{

}

void ActionOut::name_and_message(std::string a_name, Message_type a_message)
{
    char buffer[1024];
    Args arg(1, 0);
    arg.m_strings.emplace_back(a_name);
    int size = pack(buffer, arg, a_message);
    m_tcp.send_to_server(buffer, size);
}

void ActionOut::pack_and_send(Args& a_arg, Message_type a_message)
{
    char buffer[1024];
    int size = pack(buffer, a_arg, a_message);
    m_tcp.send_to_server(buffer, size);
}

void ActionOut::registration_request(std::string a_name, std::string a_password, std::string a_gender)
{
    Args arg(3, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_password);
    arg.m_strings.emplace_back(a_gender);
    pack_and_send(arg, REGISTRATION_REQUES);
}

void ActionOut::log_in_request(std::string a_name, std::string a_password, int a_amount)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_password);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send(arg, LOG_IN_REQUEST);
}

void ActionOut::start_bet(std::string a_name)
{
    name_and_message(a_name, START_BET);
}

void ActionOut::bet_action(std::string a_name, int a_chip)
{
    Args arg(1,1);
    arg.m_strings.emplace_back(a_name);
    arg.m_ints.emplace_back(a_chip);
    pack_and_send(arg, BET_ACTION);
}

void ActionOut::finish_bet(std::string a_name)
{
    name_and_message(a_name, FINISH_BET);
}

void ActionOut::check_action()
{
    name_and_message("stam", CHECK_ACTION); // just massge function
}

void ActionOut::fold_action(std::string a_name)
{
    name_and_message(a_name, FOLD_ACTION);
}

void ActionOut::wake_up_client()
{
    name_and_message("player", WAKE_UP_CLIENT);
}

void ActionOut::wake_up_server()
{
    name_and_message("player", WAKE_UP_SERVER);
}


}// poker namespace
