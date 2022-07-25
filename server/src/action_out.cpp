#include "action_out.hpp"
#include <iostream>

namespace poker
{

ActionOut::ActionOut(TcpServer& a_tcp)
: m_buffer(new char[1024])
, m_tcp(a_tcp)
{

}

ActionOut::~ActionOut()
{
    delete[] m_buffer;
}

void ActionOut::name_and_message(std::string& a_name, Message_type a_message)
{
    Args arg(1, 0);
    arg.m_strings.emplace_back(a_name);
    int size = pack(m_buffer, arg, a_message);
    m_tcp.send_all_clients(m_buffer, size);
}

void ActionOut::just_message(Message_type a_message, int a_client_socket)
{
    Args arg(1, 0); // bag to solve
    arg.m_strings.emplace_back("stam"); // bag to solve
    pack_and_send_to_client(arg, a_message, a_client_socket);
}

void ActionOut::pack_and_send_all(Args& a_arg, Message_type a_message)
{
    int size = pack(m_buffer, a_arg, a_message);
    m_tcp.send_all_clients(m_buffer, size);
}

void ActionOut::pack_and_send_to_client(Args& a_arg, Message_type a_message, int a_client_socket)
{
    int size = pack(m_buffer, a_arg, a_message);
    m_tcp.send_to_client(a_client_socket, m_buffer, size);
}

void ActionOut::flag(std::string& a_name, std::string a_flag, Message_type a_message)
{
    Args arg(2, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_flag);
    int size = pack(m_buffer, arg, a_message);
    m_tcp.send_all_clients(m_buffer, size);
}

int ActionOut::pack_player(std::string& a_name, std::string a_gender, int a_amount)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_gender);
    arg.m_ints.emplace_back(a_amount);
    return pack(m_buffer, arg, GET_PLAYER);
}

void ActionOut::registration_success(int a_client_socket)
{
    just_message(REGISTRATION_SUCCESS, a_client_socket);
}

void ActionOut::registration_duplicare_name(int a_client_socket)
{
    just_message(REGISTRATION_DUPLICATE_USER_NAME, a_client_socket);
}

void ActionOut::registration_wrong_gender(int a_client_socket)
{
    just_message(REGISTRATION_WRONG_GENDER, a_client_socket);
}

void ActionOut::log_in_success(std::string& a_name, std::string& a_gender, int a_client_socket)
{
    Args arg(2, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_gender);
    pack_and_send_to_client(arg, LOG_IN_SUCCESS, a_client_socket);
}

void ActionOut::log_in_wrong_name(int a_client_socket)
{
    just_message(LOG_IN_WRONG_USER_NAME, a_client_socket);
}

void ActionOut::log_in_wrong_password(int a_client_socket)
{
    just_message(LOG_IN_WRONG_PASSWORD, a_client_socket);
}

void ActionOut::user_name_alredy_log(int a_client_socket)
{
    just_message(USER_NAME_ALREADY_LOG, a_client_socket);
}

void ActionOut::turn_on(std::string& a_name, std::string a_flag)
{
    flag(a_name, a_flag, TURN_ON_FLAG);
}

void ActionOut::turn_off(std::string& a_name, std::string a_flag)
{
    flag(a_name, a_flag, TURN_OFF_FLAG);
}

void ActionOut::bet(std::string& a_name, int a_amount)
{
    Args arg(1, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_all(arg, BET_UPDATE);
}

void ActionOut::invalid_bet(int a_amount, int a_client_socket)
{
    Args arg(0, 1);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_to_client(arg, INVALID_BET, a_client_socket);
}

void ActionOut::check(std::string& a_name)
{
    name_and_message(a_name, CHECK_UPDATE);
}

void ActionOut::fold(std::string& a_name)
{
    name_and_message(a_name, FOLD_UPDATE);
}

void ActionOut::get_card(std::string& a_name, Card& a_card)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_card.m_suit);
    arg.m_ints.emplace_back(a_card.m_number);
    pack_and_send_all(arg, GET_CARD);
}

void ActionOut::get_chips(std::string& a_name, std::vector<int>& a_chips)
{
    int size = a_chips.size();
    Args arg(1, size);
    arg.m_strings.emplace_back(a_name);
    for(int i = 0; i < size; ++i)
        arg.m_ints.emplace_back(a_chips[i]);
    pack_and_send_all(arg, GET_CHIPS);
}

void ActionOut::clear_hand()
{
    Args arg(1, 0); // bug to solve
    arg.m_strings.emplace_back("stam"); // bug to solve
    pack_and_send_all(arg, CLEAR_HAND);
}

void ActionOut::get_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket)
{
    int size = pack_player(a_name, a_gender, a_amount);
    m_tcp.send_to_client(a_client_socket, m_buffer, size);
}

void ActionOut::get_player(std::string a_name, std::string a_gender, int a_amount)
{
    int size = pack_player(a_name, a_gender, a_amount);
    m_tcp.send_all_clients(m_buffer, size);
}

void ActionOut::delete_player(std::string a_name)
{
    name_and_message(a_name, DELETE_PLAYER);
}

void ActionOut::reveal_cards(int a_client_socket)
{
    just_message(REVEAL_CARDS, a_client_socket);
}

void ActionOut::table_get_card(Card& a_card)
{
    Args arg(1, 1);
    arg.m_strings.emplace_back(a_card.m_suit);
    arg.m_ints.emplace_back(a_card.m_number);
    pack_and_send_all(arg, TABLE_GET_CARD);
}

void ActionOut::table_get_chips(int a_amount)
{
    Args arg(0, 1);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_all(arg, TABLE_GET_CHIP); 
}

void ActionOut::table_clear_hand()
{

}

void ActionOut::table_clear_chips()
{

}

void ActionOut::wake_up_client(int a_client_socket)
{
    just_message(WAKE_UP_CLIENT, a_client_socket);
}

void ActionOut::wake_up_server()
{
    std::string server = "server";
    name_and_message(server, WAKE_UP_SERVER);
}


}// poker namespace
