#include "subscribs.hpp"
#include <iostream>

namespace poker
{


Subscribs::Subscribs(ActionOut& a_action_out)
: m_subscribs()
, m_action_out(a_action_out)
{
    load_subscribs_from_file();
}

Subscriber& Subscribs::operator[](std::string a_name)
{
    return m_subscribs[a_name];
}

void Subscribs::new_subscriber(Subscriber& a_subscriber, int a_client_socket)
{
    if(!is_valid(a_subscriber, a_client_socket))
        return;

    std::string encrypted_password = sha256(a_subscriber.m_password);
    a_subscriber.m_password = encrypted_password;
    m_subscribs[a_subscriber.m_name] = a_subscriber;
    save_subscriber_to_file(a_subscriber);
    m_action_out.registration_success(a_client_socket);
}

bool Subscribs::log_in_chack(std::string& a_name, std::string&  a_password, int a_client_socket)
{
    if(m_subscribs.find(a_name) == m_subscribs.end() || m_subscribs.find(a_name)->second.m_name == "")
    {
        m_action_out.log_in_wrong_name(a_client_socket);
        return false;
    }

    std::string encrypted_password = sha256(a_password);
    if(encrypted_password != m_subscribs[a_name].m_password)
    {
        m_action_out.log_in_wrong_password(a_client_socket);
        return false;
    }
    
    return true;
}

bool Subscribs::is_valid(Subscriber& a_subscriber, int a_client_socket)
{
    if(!is_name_valid(a_subscriber.m_name, a_client_socket))
        return false;

    if(!is_gender_valid(a_subscriber.m_gender, a_client_socket))
        return false;

    return true;
}

bool Subscribs::is_name_valid(std::string& a_name, int a_client_socket)
{
    if(m_subscribs.find(a_name) != m_subscribs.end())
    {
        m_action_out.registration_duplicare_name(a_client_socket);
        return false;
    }

    return true;
}

bool Subscribs::is_gender_valid(std::string& a_gender, int a_client_socket)
{
    if(a_gender == "male" || a_gender == "female")
        return true;

    m_action_out.registration_wrong_gender(a_client_socket);
    return false;
}

void Subscribs::load_subscribs_from_file()
{
    std::ifstream file("./resources/subscribs.txt");
    Subscriber subscriber;

    while(!file.eof())
    {
        file >> subscriber.m_name;
        file >> subscriber.m_password;
        file >> subscriber.m_gender;
        m_subscribs[subscriber.m_name] = subscriber;
    }
}

void Subscribs::save_subscriber_to_file(Subscriber& a_subscriber)
{
    std::ofstream file("./resources/subscribs.txt", std::ofstream::app);
    file << a_subscriber.m_name << std::endl;
    file << a_subscriber.m_password << std::endl;
    file << a_subscriber.m_gender << std::endl;
}



}// poker namespace
