#pragma once

#include <unordered_map>

#include "shape.hpp"
#include "hand.hpp"
#include "text.hpp"

#define TEXT_SIZE 25
#define MAX_NAME_SIZE 11
#define MAX_ACTION_SIZE 9
#define MAX_AMOUNT_DIGIT 5
#define NUM_OF_CARDS_PLAYER_HOLD 2
#define NAME_COLOR sf::Color(139, 134, 134)
#define TEXT_COLOR sf::Color(255, 228, 156)
#define ARIAL_FONT_PATH "./resources/fonts/arial.ttf"

namespace poker
{

using textPointer = std::shared_ptr<Text>;

class Player : public Shape
{
public:
    Player(std::string& a_name, std::string& a_gender, int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_frontg, int a_gap_back);
    virtual ~Player();
    
    virtual void bet(int a_amount);
    virtual void get_chip(int a_chip);
    void update_current_bet(int a_amount);

    void give_card();
    void get_card(std::string& a_suit, int a_number);

    void set_action(std::string a_action);
    virtual void turn_on_flag(std::string& a_flag);
    virtual void turn_off_flag(std::string& a_flag);
    virtual void draw_player(sf::RenderWindow& a_window);

protected:
    void set_name(std::string& a_name);
    void set_amount(std::string a_amount);
    void set_shape(std::string& a_gender, float a_scale = 1);

protected:
    int m_x;
    int m_y;
    int m_current_bet;
    int m_total_amount;

    std::unordered_map<std::string, bool> m_flags;
    std::unordered_map<std::string, textPointer> m_texts;

    Hand m_hand;
};


}// poker namespace
