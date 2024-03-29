#include "action_out.hpp"

namespace poker
{

extern bool dbg[NUM_OF_DBG_TYPES];

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

void ActionOut::name_and_amount_to_client(std::string& a_name, int a_amount, Message_type a_message, int a_client_socket)
{
    Args arg(1, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_ints.emplace_back(a_amount);
    pack_and_send_to_client(arg, a_message, a_client_socket);
}

void ActionOut::name_and_message(std::string& a_name, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    Args arg(1, 0);
    arg.m_strings.emplace_back(a_name);
    int size = pack(buffer, arg, a_message);

    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_tcp.send_all_clients(name: " << a_name << ", message: " << a_message << ")" << std::endl;

    m_tcp.send_all_clients(buffer, size, m_sockets);
}

void ActionOut::just_amount_to_client(Message_type a_message, int a_amount, int a_client_socket)
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

void ActionOut::just_message_to_client(Message_type a_message, int a_client_socket)
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

    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_tcp.send_all_clients(message: " << a_message << ")" << std::endl;

    m_tcp.send_all_clients(buffer, size, m_sockets);
}

void ActionOut::pack_and_send_to_client(Args& a_arg, Message_type a_message, int a_client_socket)
{
    char buffer[BUFFER_SIZE];
    int size = pack(buffer, a_arg, a_message);

    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_tcp.send_to_client(message: " << a_message << ", client_socket: " << a_client_socket << ")" << std::endl;

    m_tcp.send_to_client(a_client_socket, buffer, size);
}

void ActionOut::flag(std::string& a_name, std::string& a_flag, Message_type a_message)
{
    char buffer[BUFFER_SIZE];
    Args arg(2, 0);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_flag);
    int size = pack(buffer, arg, a_message);

    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_tcp.send_all_clients(name: " << a_name << ", flag: " << a_flag << ", message: " << a_message << ")" << std::endl;
    
    m_tcp.send_all_clients(buffer, size, m_sockets);
}

int ActionOut::pack_player(char* a_buffer, std::string& a_name, std::string& a_gender, int a_amount)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_gender);
    arg.m_ints.emplace_back(a_amount);
    return pack(a_buffer, arg, GET_PLAYER);
}

void ActionOut::registration_success(int a_client_socket)
{
    just_message_to_client(REGISTRATION_SUCCESS, a_client_socket);
}

void ActionOut::registration_duplicare_name(int a_client_socket)
{
    just_message_to_client(REGISTRATION_DUPLICATE_USER_NAME, a_client_socket);
}

void ActionOut::registration_wrong_gender(int a_client_socket)
{
    just_message_to_client(REGISTRATION_WRONG_GENDER, a_client_socket);
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
    just_message_to_client(LOG_IN_WRONG_USER_NAME, a_client_socket);
}

void ActionOut::log_in_wrong_password(int a_client_socket)
{
    just_message_to_client(LOG_IN_WRONG_PASSWORD, a_client_socket);
}

void ActionOut::user_name_alredy_log(int a_client_socket)
{
    just_message_to_client(USER_NAME_ALREADY_LOG, a_client_socket);
}

void ActionOut::play_success(int a_client_socket)
{
    just_message_to_client(PLAY_SUCCESS, a_client_socket);

    if(is_socket_exists(a_client_socket))
        return;

    m_sockets.emplace_back(a_client_socket); 
}

void ActionOut::view_success(int a_client_socket)
{
    just_message_to_client(VIEW_SUCCESS, a_client_socket);
}

void ActionOut::get_viewer(int a_client_socket)
{
    if(is_socket_exists(a_client_socket))
        return;

    m_sockets.emplace_back(a_client_socket); 
}

bool ActionOut::is_socket_exists(int a_client_socket)
{
    for(auto socket : m_sockets)
    {
        if(socket == a_client_socket)
            return true;
    }

    return false;
}

void ActionOut::delete_viewer(int a_client_socket)
{
    m_sockets.remove(a_client_socket);
}

void ActionOut::turn_on(std::string& a_name, std::string& a_flag)
{
    flag(a_name, a_flag, TURN_ON_FLAG);
}

void ActionOut::turn_off(std::string& a_name, std::string& a_flag)
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
    just_amount_to_client(INVALID_BET_MIN, a_amount, a_client_socket);
}

void ActionOut::invalid_bet_max(int a_amount, int a_client_socket)
{
    just_amount_to_client(INVALID_BET_MAX, a_amount, a_client_socket);
}

void ActionOut::check(std::string& a_name)
{
    name_and_message(a_name, CHECK_UPDATE);
}

void ActionOut::fold(std::string& a_name)
{
    name_and_message(a_name, FOLD_UPDATE);
}

void ActionOut::get_card(std::string& a_name, cardPointer a_card)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_card.get()->m_suit);
    arg.m_ints.emplace_back(a_card.get()->m_number);
    pack_and_send_all(arg, GET_CARD);
}

void ActionOut::get_card(std::string& a_name, cardPointer a_card, int a_socket)
{
    Args arg(2, 1);
    arg.m_strings.emplace_back(a_name);
    arg.m_strings.emplace_back(a_card.get()->m_suit);
    arg.m_ints.emplace_back(a_card.get()->m_number);
    pack_and_send_to_client(arg, GET_CARD, a_socket);
}

void ActionOut::get_chips(std::string& a_name, int a_chip)
{
    name_and_amount(a_name, a_chip, GET_CHIP);
}

void ActionOut::give_card(std::string& a_name)
{
    name_and_message(a_name, GIVE_CARD);
}

void ActionOut::get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket)
{
    char buffer[BUFFER_SIZE];
    int size = pack_player(buffer, a_name, a_gender, a_amount);

    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_tcp.send_to_client(name: " << a_name << ", gender: " << a_gender << ", amount: " << a_amount << ", client_socket: " << a_client_socket << ")" << std::endl;
    
    m_tcp.send_to_client(a_client_socket, buffer, size);
}

void ActionOut::get_player(std::string& a_name, std::string& a_gender, int a_amount)
{
    char buffer[BUFFER_SIZE];
    int size = pack_player(buffer, a_name, a_gender, a_amount);

    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_tcp.send_all_clients(name: " << a_name << ", gender: " << a_gender << ", amount: " << a_amount << ")" << std::endl;
    
    m_tcp.send_all_clients(buffer, size, m_sockets);    
}

void ActionOut::delete_player(std::string& a_name, int a_socket)
{
    if(dbg[ACTION_OUT])[[unlikely]]
        std::cout << __func__ << "(): call m_sockets.remove(a_socket = " << a_socket << ")" << "\n";

    m_sockets.remove(a_socket);
    name_and_message(a_name, DELETE_PLAYER);
}

void ActionOut::reveal_cards(std::string& a_name)
{
    name_and_message(a_name, REVEAL_CARDS);
}

void ActionOut::table_is_full(int a_client_socket)
{
    just_message_to_client(TABLE_FULL, a_client_socket);
}

void ActionOut::table_is_empty(int a_client_socket)
{
    just_message_to_client(TABLE_EMPTY, a_client_socket);
}

void ActionOut::table_get_card(cardPointer a_card)
{
    name_and_amount(a_card.get()->m_suit, a_card.get()->m_number, TABLE_GET_CARD);
}

void ActionOut::table_get_chip(int a_amount)
{
    just_amount_to_all(TABLE_GET_CHIP, a_amount); 
}

void ActionOut::table_get_card(cardPointer a_card, int a_client_socket)
{
    name_and_amount_to_client(a_card.get()->m_suit, a_card.get()->m_number, TABLE_GET_CARD, a_client_socket);
}

void ActionOut::table_get_chip(int a_amount, int a_client_socket)
{
    just_amount_to_client(TABLE_GET_CHIP, a_amount, a_client_socket); 
}

void ActionOut::table_give_card()
{
    just_message_to_all(TABLE_GIVE_CARD);
}

void ActionOut::table_clear_wallet()
{
    just_message_to_all(TABLE_CLEAR_WALLET);
}

void ActionOut::table_give_chip(int a_amount)
{
    just_amount_to_all(TABLE_GIVE_CHIP, a_amount); 
}

void ActionOut::wake_up_client(int a_client_socket)
{
    just_message_to_client(WAKE_UP_CLIENT, a_client_socket);
}

void ActionOut::wake_up_server()
{
    just_message_to_all(WAKE_UP_SERVER);
}

void ActionOut::clear_action(std::string& a_name)
{
    name_and_message(a_name, CLEAR_ACTION);
}

void ActionOut::round_winer(std::vector<std::string>& a_name)
{
    char buffer[BUFFER_SIZE];
    int num_of_winners = a_name.size();
    Args arg(num_of_winners, 0);

    for(auto winner : a_name)
        arg.m_strings.emplace_back(winner);

    int size = pack(buffer, arg, ROUND_WINER);
    m_tcp.send_all_clients(buffer, size, m_sockets);
}

void ActionOut::game_winer(std::string& a_name)
{
    name_and_message(a_name, GAME_WINER);
}

void ActionOut::clear_text()
{
    just_message_to_all(CLEAR_TEXT);
}

void ActionOut::clear_text(int a_client_socket)
{
    just_message_to_client(CLEAR_TEXT, a_client_socket);
}

void ActionOut::clear_screen(int a_client_socket)
{
    just_message_to_client(CLEAR_SCREEN, a_client_socket);
}

void  ActionOut::no_more_tables(int a_client_socket)
{
    just_message_to_client(NO_MORE_TABLES, a_client_socket);
}

void ActionOut::moves_to_another_table(int a_client_socket)
{
    just_message_to_client(MOVES_TO_ANOTHER_TABLE, a_client_socket);
}

void ActionOut::print_result(std::string& a_name, int a_result)
{
    name_and_amount(a_name, a_result, RESULT);
}


}// poker namespace
