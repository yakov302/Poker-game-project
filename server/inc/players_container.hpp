#pragma once

#include <condition_variable>
#include <unordered_map>
#include <memory>
#include "player.hpp"
#include "action_out.hpp"

namespace poker
{

using playerPointer = std::shared_ptr<Player>;
using playerIterator = std::unordered_map<std::string, poker::playerPointer>::iterator;

class PlayersContainer
{
public:
    PlayersContainer(ActionOut& a_action_out);

    void new_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket);
    void delete_player(std::string& a_name);
    void delete_player(int a_client_socket);
    void decrease (std::string& a_name, int a_amount);
    void increase (std::string& a_name, int a_amount);
    void get_card(std::string& a_name, Card a_card);
    void clear_hand(std::string& a_name);
    void turn_on(std::string& a_name, std::string a_flag);
    void turn_off(std::string& a_name, std::string a_flag);
    std::condition_variable& cond_var();

    bool log_in_chack(std::string a_name, int a_client_socket)const;
    playerIterator begin();
    playerIterator end();
    int num_of_players()const;

private:
    ActionOut& m_action_out;
    std::condition_variable m_cond_var;
    std::unordered_map<std::string, playerPointer> m_players;
};


}// poker namespace
