# pragma once

#include <fstream>
#include <unordered_map>

#include "sha256.hpp"
#include "action_out.hpp"

namespace poker
{

struct Subscriber
{
    std::string m_name;
    std::string m_password;
    std::string m_gender;
};

class Subscribs
{
public:
    Subscribs(ActionOut& a_action_out);

    Subscriber& operator[](std::string a_name);
    void new_subscriber(Subscriber& a_subscriber, int a_client_socket);
    bool log_in_chack(std::string& a_name, std::string&  a_password, int a_client_socket);

private:
    bool is_valid(Subscriber& a_subscriber, int a_client_socket);
    bool is_name_valid(std::string& a_name, int a_client_socket);
    bool is_gender_valid(std::string& a_gender, int a_client_socket);
    void save_subscriber_to_file(Subscriber& a_subscriber);
    void load_subscribs_from_file();

private:
    ActionOut& m_action_out;
    std::unordered_map<std::string, Subscriber> m_subscribs;
};


}// poker namespace
