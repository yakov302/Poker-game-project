#pragma once

#include <memory>
#include <mutex>

#include "card.hpp"
#include "cards_container.hpp"

namespace poker
{

#define BACK_CARD_GAP 5
#define BACK_CARD_IMAGE_PATH "./resources/images/cards/back.png"

using Lock = std::unique_lock<std::mutex>;
using cardPointer = std::shared_ptr<Card>;

class Hand 
{
public:
    Hand(int a_num_of_cards_front, int a_num_of_cards_back, int a_x, int a_y, float a_scale, int a_gap_front, int a_gap_back);

    void push(std::string& a_suit, int a_number);
    void set_front_position(int a_x, int a_y);
    void pop();

    void draw_front(sf::RenderWindow& a_window)const;
    void draw_back(sf::RenderWindow& a_window)const;
    bool is_in_back_range(int a_x, int a_y)const;

   void clear();

private:
    int m_x;
    int m_y;
    int m_gap_between_cards;
    mutable std::mutex m_mutex;
    std::vector<cardPointer> m_cards;
    std::vector<cardPointer> m_back; 
};


}// poker namespace
