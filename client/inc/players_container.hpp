#pragma once

#include <unordered_map>

#include"player.hpp"
#include"self.hpp"

namespace poker
{

using playerPointer = std::shared_ptr<Player>;

class PlayersContainer 
{
public:
    PlayersContainer();

    void get_player(std::string& a_name, std::string& a_gender, int a_amount, int a_x_self = 0 , int a_y_self = 50, int a_x_card = 0, int a_y_card = 0, float a_scale_card = 0.5, int a_gap_of_printing = 5);
    void get_player(std::string& a_name, Self& a_player);
    void delete_player(std::string& a_name);

    void get_chips(std::string& a_name, std::vector<int>& a_chips);
    void bet(std::string& a_name, int a_amount);

    void get_card(std::string& a_name, std::string& a_suit, int a_number);
    void clear_hand(std::string& a_name);

    void set_action(std::string& a_name, std::string a_action);
    void update_current_bet(std::string& a_name, int a_amount);
    void turn_on_flag(std::string& a_name, std::string a_flag);
    void turn_off_flag(std::string& a_name, std::string a_flag);
    void draw_Players(sf::RenderWindow& a_window);

private:
    int m_num_of_players;
    std::unordered_map<std::string, playerPointer> m_players;
};


}// poker namespace
