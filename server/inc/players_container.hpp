#pragma once

#include <stdio.h>
#include <memory>

#include "deck.hpp"
#include "wait.hpp"
#include "player.hpp"
#include "action_out.hpp"

#define MAX_NUM_OF_PLAYERS 6

namespace poker
{

using cardPointer = std::shared_ptr<Card>;
using playerPointer = std::shared_ptr<Player>;
using playerIterator = std::unordered_map<std::string, poker::playerPointer>::iterator;

class PlayersContainer
{
public:
    PlayersContainer(ActionOut& a_action_out);

    void new_player(std::string& a_name, std::string& a_gender, int a_amount, int a_client_socket);
    bool log_in_chack(std::string& a_name, int a_client_socket)const;
    void delete_player(int a_client_socket);
    int num_of_players()const;

    void take_chip (std::string& a_name, int a_chip);
    int get (std::string& a_name, std::string& a_var);
    void set (std::string& a_name, std::string& a_var, int a_value);

    cardPointer give_card(std::string& a_name);
    bool is_it_has_a_cards(std::string& a_name);
    void get_card(std::string& a_name, cardPointer a_card);
    std::pair<cardPointer,cardPointer> show_cards(std::string& a_name);

    void turn_on(std::string& a_name, std::string& a_flag);
    void turn_off(std::string& a_name, std::string& a_flag);
    bool is_flag_on(std::string& a_name, std::string& a_flag) ;

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
