#pragma once

#include <memory>
#include <unordered_map>

#include "deck.hpp"
#include "wait.hpp"
#include "player.hpp"
#include "action_out.hpp"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;
using playerPointer = std::shared_ptr<Player>;
using playerIterator = std::unordered_map<std::string, poker::playerPointer>::iterator;

class PlayersContainer
{
public:
    PlayersContainer(ActionOut& a_action_out);

    void new_player(std::string a_name, std::string a_gender, int a_amount, int a_client_socket);
    void delete_player(int a_client_socket);

    void decrease (std::string& a_name, int a_amount);
    void increase (std::string& a_name, int a_amount);

    void get_card(std::string& a_name, cardPointer a_card);
    cardPointer give_card(std::string& a_name);

    void turn_on(std::string& a_name, std::string a_flag);
    void turn_off(std::string& a_name, std::string a_flag);

    bool is_it_has_a_cards(std::string& a_name);
    bool is_flag_on(std::string& a_name, std::string a_flag) ;

    void set_bet(std::string& a_name, int a_amount);
    void set_result(std::string& a_name, int a_result);

    int bet (std::string& a_name);
    int result(std::string& a_name);
    int amount(std::string& a_name);
    cardPointer first_card(std::string& a_name);
    cardPointer second_card(std::string& a_name);

    int num_of_players()const;
    bool log_in_chack(std::string a_name, int a_client_socket)const;

    Wait& wait();
    playerIterator end();
    playerIterator begin();

private:
    void delete_player(std::string& a_name);

private:
    Wait m_wait;
    ActionOut& m_action_out;
    std::unordered_map<std::string, playerPointer> m_players;
};


}// poker namespace
