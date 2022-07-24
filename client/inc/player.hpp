#pragma once

#include"shape.hpp"
#include"hand.hpp"
#include"text.hpp"

namespace poker
{

using textPointer = std::shared_ptr<Text>;

class Player : public Shape
{
public:
    Player(std::string a_name, std::string a_gender, int a_amount, int a_x_self, int a_y_self, int a_x_card, int a_y_card, float a_scale_card, int a_gap_of_printing);

    virtual void get_chips(std::vector<int>& a_chips);
    virtual void bet(int a_amount);

    void get_card(std::string& a_suit, int a_number);
    void clear_hand();

    void set_action(std::string a_action);
    virtual void turn_on_flag(std::string a_flag);
    virtual void turn_off_flag(std::string a_flag);
    virtual void draw_player(sf::RenderWindow& a_window);

    void set_shape(sf::Sprite& a_shape, sf::Texture& a_texture, std::string& a_gender, int a_x, int a_y);
    void set_name(std::unordered_map<std::string, textPointer>& a_texts, std::string& a_name, int a_x, int a_y, int a_amount);

    int m_x;
    int m_y;
    Hand m_hand;
    int m_current_bet;
    int m_total_amount;
    std::unordered_map<std::string, bool> m_flags;
    std::unordered_map<std::string, textPointer> m_texts;
};


}// poker namespace
