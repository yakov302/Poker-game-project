#pragma once

#include"player.hpp"
#include"self.hpp"

#define PLAYER_SIZE 300

namespace poker
{

using playerPointer = std::shared_ptr<Player>;

class PlayersContainer 
{
public:
    PlayersContainer();
    ~PlayersContainer();

    void get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_x_self = 0 , int a_y_self = 50, int a_x_card = 0, int a_y_card = 0, float a_scale_card = 1, int a_gap_front = 25, int a_gap_back = 5);
    void get_player(std::string& a_name, Self& a_player);
    void delete_player(std::string& a_name);

    void get_chip(std::string& a_name, int a_chip);
    void bet(std::string& a_name, int a_amount);

    void get_card(std::string& a_name, std::string& a_suit, int a_number);
    void give_card(std::string& a_name);

    void set_action(std::string& a_name, std::string a_action);
    void update_current_bet(std::string& a_name, int a_amount);
    void turn_on_flag(std::string& a_name, std::string a_flag);
    void turn_off_flag(std::string& a_name, std::string a_flag);
    void draw_Players(sf::RenderWindow& a_window);

private:
    bool* m_locations;
    int m_num_of_players;
    std::unordered_map<std::string, playerPointer> m_players;
};


}// poker namespace
