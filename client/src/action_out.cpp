#include "action_out.hpp"
#include <iostream>

namespace poker
{

ActionOut::ActionOut(TcpClient& a_tcp)
: m_buffer(new char[1024])
, m_tcp(a_tcp)
{

}

ActionOut::~ActionOut()
{
    delete[] m_buffer;
    m_buffer = nullptr;
}

void ActionOut::name_and_message(std::string a_name, Message_type a_message)
{
    Args arg(1, 0);
    arg.m_strings.emplace_back(a_name);
    int size = pack(m_buffer, arg, a_message);
    m_tcp.send_to_server(m_buffer, size);
}

void ActionOut::registration_request(std::string a_name, std::string a_password, std::string a_gender)
{
    Args arg(3, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_password);
    arg.m_strings.emplace_back(a_gender);
    int size = pack(m_buffer, arg, REGISTRATION_REQUES);
    m_tcp.send_to_server(m_buffer, size);
}

void ActionOut::log_in_request(std::string a_name, std::string a_password, int a_amount)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_password);
    arg.m_ints.emplace_back(a_amount);
    int size = pack(m_buffer, arg, LOG_IN_REQUEST);
    m_tcp.send_to_server(m_buffer, size);
}

void ActionOut::bet_action(std::string a_name, int a_chip)
{
    Args args(1,1);
    args.m_strings.emplace_back(a_name);
    args.m_ints.emplace_back(a_chip);
    int size = pack(m_buffer, args, BET_ACTION);
    m_tcp.send_to_server(m_buffer, size);
}

void ActionOut::finish_bet(std::string a_name)
{
    name_and_message(a_name, FINISH_BET);
}

void ActionOut::check_action(std::string a_name)
{
    name_and_message(a_name, CHECK_ACTION);
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
