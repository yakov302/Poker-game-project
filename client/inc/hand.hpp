#pragma once

#include <memory>
#include <mutex>

#include "card.hpp"

namespace poker
{

using Lock = std::unique_lock<std::mutex>;
using cardPointer = std::shared_ptr<Card>;

class Hand 
{
public:
   Hand(int a_num_of_cards_front, int a_num_of_cards_back, int a_x, int a_y, float a_scale, int a_gap_of_prints);

   void push(std::string& a_suit, int a_number);
   void push(std::vector<std::pair<std::string, int>> a_cards);
   void clear();

   void draw_front(sf::RenderWindow& a_window)const;
   void draw_back(sf::RenderWindow& a_window)const;
   bool is_in_back_range(int a_x, int a_y)const;

private:
    int m_x;
    int m_y;
    int m_gap_of_prints;
    mutable std::mutex m_mutex;
    std::vector<cardPointer> m_cards;
    std::vector<cardPointer> m_back; 
};


}// poker namespace
