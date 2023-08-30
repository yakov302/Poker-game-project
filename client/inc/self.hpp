#pragma once

#include"player.hpp"
#include"wallet.hpp"

#define VIEW 2
#define HAND_X_POS 1260
#define HAND_Y_POS 630
#define HAND_SCALE 1.8
#define SELF_X_POS 1060
#define SELF_Y_POS 650
#define SELF_WALLET_X_POS 880
#define SELF_WALLET_Y_POS 750
#define GAP_BETWEEN_FRONT_CARDS 30 
#define GAP_BETWEEN_BACK_CARDS 15

namespace poker
{

class Self : public Player
{
public:
    Self(std::string a_name, std::string a_gender, int a_amount);
    
    int current_bet();
    int bet(int a_x, int a_y);
    int bet_for_all_in(int amount);
    bool exchange(int a_x, int a_y);

    void bet(int amount) override;
    void get_chip(int  a_chip) override;
    void draw_player(sf::RenderWindow& a_window) override;

    int amount()const;
    const std::string& name();
    bool is_in_back_range(int a_x, int a_y)const;
    bool is_in_wallet_range(int a_x, int a_y)const;
    void draw_hand_front(sf::RenderWindow& a_window)const;

    bool is_flag_on(std::string& a_flag);
    void set_name_and_gender(std::string& a_name, std::string& a_gender);

private:
    Wallet m_wallet;
    std::string m_name;
};


}// poker namespace
