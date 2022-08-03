#pragma once

#include "tcp_server.hpp"
#include "protocol.hpp"
#include "card.hpp"

#define BUFFER_SIZE 2048

namespace poker
{

class ActionOut
{
public:
    ActionOut(TcpServer& a_tcp);

    void registration_success(int a_client_socket);
    void registration_duplicare_name(int a_client_socket);
    void registration_wrong_gender(int a_client_socket);
    void log_in_success(std::string& a_name, std::string& a_genderint, int a_client_socket);
    void log_in_wrong_name(int a_client_socket);
    void log_in_wrong_password(int a_client_socket);
    void user_name_alredy_log(int a_client_socket);
    void turn_on(std::string& a_name, std::string a_flag);
    void turn_off(std::string& a_name, std::string a_flag);
    void start_bet(std::string& a_name, int a_amount);
    void bet(std::string& a_name, int a_amount);
    void invalid_bet_min(int a_amount, int a_client_socket);
    void invalid_bet_max(int a_amount, int a_client_socket);
    void check(std::string& a_name);
    void fold(std::string& a_name);
    void clear_fold(std::string& a_name);
    void get_card(std::string& a_name, Card& a_card);
    void get_chips(std::string& a_name, int a_chip);
    void give_card(std::string& a_name);
    void get_player(std::string a_name, std::string a_gender, int a_amount);
    void get_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket);
    void delete_player(std::string a_name);
    void reveal_cards(std::string& a_name);
    void table_get_card(Card& a_card);
    void table_get_chip(int a_amount);
    void table_give_card();
    void table_give_chip(int a_amount);
    void wake_up_client(int a_client_socket);
    void wake_up_server();
    void clear_action(std::string& a_name);
    void round_winer(std::string& a_name);
    void game_winer(std::string& a_name);
    void clear_text();
    void print_result(std::string& a_name, int a_result);

private:
    void flag(std::string& a_name, std::string a_flag, Message_type a_message);
    void name_and_message(std::string& a_name, Message_type a_message);
    void just_message_to_clienet(Message_type a_message, int a_client_socket);
    void just_message_to_all(Message_type a_message);
    void pack_and_send_all(Args& a_arg, Message_type a_message);
    void pack_and_send_to_client(Args& a_arg, Message_type a_message, int a_client_socket);
    int pack_player(char* a_buffer, std::string& a_name, std::string a_gender, int a_amount);

private:
    TcpServer& m_tcp;
};


}// poker namespace
