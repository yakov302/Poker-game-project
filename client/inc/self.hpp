#pragma once

#include"player.hpp"
#include"wallet.hpp"

namespace poker
{

class Self : public Player
{
public:
    Self(std::string a_name, std::string a_gender, int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_front, int a_gap_back);
    
    int bet(int a_x, int a_y);
    bool exchange(int a_x, int a_y);

    void bet(int amount) override;
    void get_chip(int  a_chip) override;
    void draw_player(sf::RenderWindow& a_window) override;

    int amount()const;
    std::string name()const;
    bool is_in_back_range(int a_x, int a_y)const;
    bool is_in_wallet_range(int a_x, int a_y)const;
    void draw_hand_front(sf::RenderWindow& a_window)const;

    bool is_flag_on(std::string a_flag);
    void set_name_and_gender(std::string a_name, std::string a_gender);

private:
    Wallet m_wallet;
    std::string m_name;
};


}// poker namespace
