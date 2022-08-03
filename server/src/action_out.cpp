#include "action_out.hpp"

namespace poker
{

ActionOut::ActionOut(TcpServer& a_tcp)
: m_tcp(a_tcp)
{

}

void ActionOut::name_and_amount(std::string& a_name, int a_amount, Message_type a_message)
{
    Args arg(1, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_all(arg, a_message);
}

void ActionOut::name_and_message(std::string& a_name, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    Args arg(1, 0);
    arg.m_strings.emplace_back(a_name);
    int size = pack(buffer, arg, a_message);
    m_tcp.send_all_clients(buffer, size);
}

void ActionOut::just_amount_to_clienet(Message_type a_message, int a_amount, int a_client_socket)
{
    Args arg(0, 1);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_to_client(arg, a_message, a_client_socket);
}

void ActionOut::just_amount_to_all(Message_type a_message, int a_amount)
{
    Args arg(0, 1);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_all(arg, a_message); 
}

void ActionOut::just_message_to_clienet(Message_type a_message, int a_client_socket)
{
    Args arg(0, 0);
    pack_and_send_to_client(arg, a_message, a_client_socket);
}

void ActionOut::just_message_to_all(Message_type a_message)
{
    Args arg(0, 0);
    pack_and_send_all(arg, a_message);
}

void ActionOut::pack_and_send_all(Args& a_arg, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    int size = pack(buffer, a_arg, a_message);
    m_tcp.send_all_clients(buffer, size);
}

void ActionOut::pack_and_send_to_client(Args& a_arg, Message_type a_message, int a_client_socket)
{
    char buffer[BUFFER_SIZE];
    int size = pack(buffer, a_arg, a_message);
    m_tcp.send_to_client(a_client_socket, buffer, size);
}

void ActionOut::flag(std::string& a_name, std::string a_flag, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    Args arg(2, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_flag);
    int size = pack(buffer, arg, a_message);
    m_tcp.send_all_clients(buffer, size);
}

int ActionOut::pack_player(char* a_buffer, std::string& a_name, std::string a_gender, int a_amount)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_gender);
    arg.m_ints.emplace_back(a_amount);
    return pack(a_buffer, arg, GET_PLAYER);
}

void ActionOut::registration_success(int a_client_socket)
{
    just_message_to_clienet(REGISTRATION_SUCCESS, a_client_socket);
}

void ActionOut::registration_duplicare_name(int a_client_socket)
{
    just_message_to_clienet(REGISTRATION_DUPLICATE_USER_NAME, a_client_socket);
}

void ActionOut::registration_wrong_gender(int a_client_socket)
{
    just_message_to_clienet(REGISTRATION_WRONG_GENDER, a_client_socket);
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
    just_message_to_clienet(LOG_IN_WRONG_USER_NAME, a_client_socket);
}

void ActionOut::log_in_wrong_password(int a_client_socket)
{
    just_message_to_clienet(LOG_IN_WRONG_PASSWORD, a_client_socket);
}

void ActionOut::user_name_alredy_log(int a_client_socket)
{
    just_message_to_clienet(USER_NAME_ALREADY_LOG, a_client_socket);
}

void ActionOut::turn_on(std::string& a_name, std::string a_flag)
{
    flag(a_name, a_flag, TURN_ON_FLAG);
}

void ActionOut::turn_off(std::string& a_name, std::string a_flag)
{
    flag(a_name, a_flag, TURN_OFF_FLAG);
}

void ActionOut::start_bet(std::string& a_name, int a_amount)
{
    name_and_amount(a_name, a_amount, START_BET_UPDATE);
}

void ActionOut::bet(std::string& a_name, int a_amount)
{
    name_and_amount(a_name, a_amount, BET_UPDATE);
}

void ActionOut::invalid_bet_min(int a_amount, int a_client_socket)
{
    just_amount_to_clienet(INVALID_BET_MIN, a_amount, a_client_socket);
}

void ActionOut::invalid_bet_max(int a_amount, int a_client_socket)
{
    just_amount_to_clienet(INVALID_BET_MAX, a_amount, a_client_socket);
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

void ActionOut::get_chips(std::string& a_name, int a_chip)
{
    name_and_amount(a_name, a_chip, GET_CHIP);
}

void ActionOut::give_card(std::string& a_name)
{
    name_and_message(a_name, GIVE_CARD);
}

void ActionOut::get_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket)
{
    char buffer[BUFFER_SIZE];
    int size = pack_player(buffer, a_name, a_gender, a_amount);
    m_tcp.send_to_client(a_client_socket, buffer, size);
}

void ActionOut::get_player(std::string a_name, std::string a_gender, int a_amount)
{
    char buffer[BUFFER_SIZE];
    int size = pack_player(buffer, a_name, a_gender, a_amount);
    m_tcp.send_all_clients(buffer, size);
}

void ActionOut::delete_player(std::string a_name)
{
    name_and_message(a_name, DELETE_PLAYER);
}

void ActionOut::reveal_cards(std::string& a_name)
{
    name_and_message(a_name, REVEAL_CARDS);
}

void ActionOut::table_get_card(Card& a_card)
{
    name_and_amount(a_card.m_suit, a_card.m_number, TABLE_GET_CARD);
}

void ActionOut::table_get_chip(int a_amount)
{
    just_amount_to_all(TABLE_GET_CHIP, a_amount); 
}

void ActionOut::table_give_card()
{
    just_message_to_all(TABLE_GIVE_CARD);
}

void ActionOut::table_give_chip(int a_amount)
{
    just_amount_to_all(TABLE_GIVE_CHIP, a_amount); 
}

void ActionOut::wake_up_client(int a_client_socket)
{
    just_message_to_clienet(WAKE_UP_CLIENT, a_client_socket);
}

void ActionOut::wake_up_server()
{
    just_message_to_all(WAKE_UP_SERVER);
}

void ActionOut::clear_action(std::string& a_name)
{
    name_and_message(a_name, CLEAR_ACTION);
}

void ActionOut::round_winer(std::string& a_name)
{
    name_and_message(a_name, ROUND_WINER);
}

void ActionOut::game_winer(std::string& a_name)
{
    name_and_message(a_name, GAME_WINER);
}

void ActionOut::clear_text()
{
    just_message_to_all(CLEAR_TEXT);
}

void ActionOut::print_result(std::string& a_name, int a_result)
{
    name_and_amount(a_name, a_result, RESULT);
}


}// poker namespace
