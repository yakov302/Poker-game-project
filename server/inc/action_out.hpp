#pragma once

#include <memory>

#include "tcp_server.hpp"
#include "protocol.hpp"
#include "cli_cmd.hpp"
#include "card.hpp"

#define BUFFER_SIZE 512

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class ActionOut
{
public:
    ActionOut(TcpServer& a_tcp);

    void registration_success(int a_client_socket);
    void registration_wrong_gender(int a_client_socket);
    void registration_duplicare_name(int a_client_socket);

    void log_in_wrong_name(int a_client_socket);
    void user_name_alredy_log(int a_client_socket);
    void log_in_wrong_password(int a_client_socket);
    void log_in_success(std::string& a_name, std::string& a_gender, int a_client_socket);

    void turn_on(std::string& a_name, std::string& a_flag);
    void turn_off(std::string& a_name, std::string& a_flag);

    void bet(std::string& a_name, int a_amount);
    void start_bet(std::string& a_name, int a_amount);
    void invalid_bet_min(int a_amount, int a_client_socket);
    void invalid_bet_max(int a_amount, int a_client_socket);

    void fold(std::string& a_name);
    void check(std::string& a_name);

    void give_card(std::string& a_name);
    void reveal_cards(std::string& a_name);
    void get_chips(std::string& a_name, int a_chip);
    void get_card(std::string& a_name, cardPointer a_card);

    void delete_player(std::string& a_name, int a_socket);
    void get_player(std::string& a_name, std::string& a_gender, int a_amount);
    void get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket);
    
    void table_give_card();
    void table_clear_wallet();
    void table_get_card(cardPointer a_card);
    void table_get_chip(int a_amount);
    void table_give_chip(int a_amount);

    void wake_up_server();
    void wake_up_client(int a_client_socket);

    void clear_text();
    void game_winer(std::string& a_name);
    void round_winer(std::string& a_name);
    void clear_action(std::string& a_name);
    void print_result(std::string& a_name, int a_result);

private:
    void name_and_message(std::string& a_name, Message_type a_message);
    void flag(std::string& a_name, std::string& a_flag, Message_type a_message);
    void name_and_amount(std::string& a_name, int a_amount, Message_type a_message);

    void just_message_to_all(Message_type a_message);
    void just_amount_to_all(Message_type a_message, int a_amount);
    void just_message_to_clienet(Message_type a_message, int a_client_socket);
    void just_amount_to_clienet(Message_type a_message, int a_amount, int a_client_socket);

    void pack_and_send_all(Args& a_arg, Message_type a_message);
    void pack_and_send_to_client(Args& a_arg, Message_type a_message, int a_client_socket);
    int pack_player(char* a_buffer, std::string& a_name, std::string& a_gender, int a_amount);

private:
    TcpServer& m_tcp;
    std::list<int> m_sockets;
};


}// poker namespace
