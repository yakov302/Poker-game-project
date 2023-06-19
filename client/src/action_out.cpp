#include "action_out.hpp"

namespace poker
{

ActionOut::ActionOut(TcpClient& a_tcp)
: m_tcp(a_tcp)
{

}

void ActionOut::just_message(Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    Args arg(0, 0);
    int size = pack(buffer, arg, a_message);
    m_tcp.send_to_server(buffer, size);
}

void ActionOut::name_and_message(const std::string& a_name, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    Args arg(1, 0);
    arg.m_strings.emplace_back(a_name);
    int size = pack(buffer, arg, a_message);
    m_tcp.send_to_server(buffer, size);
}

void ActionOut::pack_and_send(Args& a_arg, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    int size = pack(buffer, a_arg, a_message);
    m_tcp.send_to_server(buffer, size);
}

void ActionOut::registration_request(const std::string& a_name, const std::string& a_password, const std::string& a_gender)
{
    Args arg(3, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_password);
    arg.m_strings.emplace_back(a_gender);
    pack_and_send(arg, REGISTRATION_REQUES);
}

void ActionOut::log_in_request(const std::string& a_name, const std::string& a_password, int a_amount)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_password);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send(arg, LOG_IN_REQUEST);
}

void ActionOut::start_bet(const std::string& a_name)
{
    name_and_message(a_name, START_BET_ACTION);
}

void ActionOut::bet_action(const std::string& a_name, int a_chip)
{
    Args arg(1,1);
    arg.m_strings.emplace_back(a_name);
    arg.m_ints.emplace_back(a_chip);
    pack_and_send(arg, BET_ACTION);
}

void ActionOut::finish_bet(const std::string& a_name)
{
    name_and_message(a_name, FINISH_BET);
}

void ActionOut::check_action()
{
    just_message(CHECK_ACTION);
}

void ActionOut::fold_action()
{
    just_message(FOLD_ACTION); 
}

void ActionOut::wake_up_client()
{
    just_message(WAKE_UP_CLIENT);
}

void ActionOut::wake_up_server()
{
    just_message(WAKE_UP_SERVER);
}


}// poker namespace
