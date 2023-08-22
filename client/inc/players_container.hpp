#pragma once

#include<vector>

#include"player.hpp"
#include"self.hpp"

#define CARD_Y_POS 90
#define CARDS_SCLAE 1
#define PLAYER_SIZE 300
#define PLAYERS_Y_POS 50
#define GAP_BETWEEN_BACK_CARDS_ 5
#define GAP_BETWEEN_FRONT_CARDS_ 25 
#define MAX_NUM_OF_PAYERS_IN_TABLE 5
#define PLAYERS_X_POS 230
#define GAP_BETWEEN_CARDS_AND_PLAYER 140

namespace poker
{

using playerPointer = std::shared_ptr<Player>;

class PlayersContainer 
{
public:
    PlayersContainer();

    void get_player(std::string& a_name, std::string& a_gender, int a_amount);
    void get_player(std::string& a_name, Self& a_player);
    void delete_player(std::string& a_name);

    void get_chip(std::string& a_name, int a_chip);
    void bet(std::string& a_name, int a_amount);

    void get_card(std::string& a_name, std::string& a_suit, int a_number);
    void give_card(std::string& a_name);

    void set_action(std::string& a_name, std::string a_action);
    void update_current_bet(std::string& a_name, int a_amount);
    void turn_on_flag(std::string& a_name, std::string& a_flag);
    void turn_off_flag(std::string& a_name, std::string& a_flag);
    void draw_Players(sf::RenderWindow& a_window);

    void clear();
    
private:
    void find_empty_place(int& a_i, std::string& a_name);
    void set_as_empty_place(std::string& a_name, int num_of_players);

private:
    int m_num_of_players;
    std::vector<std::string> m_locations;
    std::unordered_map<std::string, playerPointer> m_players;
};


}// poker namespace
