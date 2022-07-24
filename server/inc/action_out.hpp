#pragma once

#include "tcp_server.hpp"
#include "protocol.hpp"

namespace poker
{

class ActionOut
{
public:
    ActionOut(TcpServer& a_tcp);
    ~ActionOut();

    void registration_success(int a_client_socket);
    void registration_duplicare_name(int a_client_socket);
    void registration_wrong_gender(int a_client_socket);
    void log_in_success(std::string& a_name, std::string& a_genderint, int a_client_socket);
    void log_in_wrong_name(int a_client_socket);
    void log_in_wrong_password(int a_client_socket);
    void turn_on(std::string& a_name, std::string a_flag);
    void turn_off(std::string& a_name, std::string a_flag);
    void bet(Args& a_arg);
    void check(std::string& a_name);
    void fold(std::string& a_name);
    void get_card();
    void get_chips();
    void clear_hand();
    void get_player(std::string a_name, std::string a_gender, int a_amount);
    void get_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket);
    void delete_player(std::string a_name);
    void reveal_cards();
    void table_get_card();
    void table_get_chips(Args& a_arg);
    void table_clear_hand();
    void table_clear_chips();
    void wake_up(int a_client_socket);

private:
    void flag(std::string& a_name, std::string a_flag, Message_type a_message);
    void name_and_message(std::string& a_name, Message_type a_message);
    void just_message(Message_type a_message, int a_client_socket);
    void pack_and_send_all(Args& a_arg, Message_type a_message);
    int pack_player(std::string& a_name, std::string a_gender, int a_amount);

private:
    char* m_buffer;
    TcpServer& m_tcp;
};


}// poker namespace
