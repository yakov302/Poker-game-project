#include "hand.hpp"

#include <iostream>
namespace poker
{

namespace impl
{

void init_back(std::vector<cardPointer>& a_back, int& a_num_of_cards_back, int& a_x, int& a_y, float a_scale, int& a_gap)
{
    int x = a_x - a_gap;
    for(int i = 0; i < a_num_of_cards_back; ++i)
    {
        a_back.emplace_back(cardPointer(new Card("none", 0, "./resources/images/cards/back.png")));
        a_back[i].get()->set_scale(a_scale, a_scale);
        a_back[i].get()->set_position(x += a_gap, a_y);
    }
}

void set_position(std::vector<cardPointer>& a_cards, int& a_x, int& a_y, int& a_gap)
{
    int x = a_x - a_gap - 10;
    for(auto& card : a_cards)
       card.get()->set_position(x += a_gap + 10, a_y - 10);
}


}//namespace impl

Hand::Hand(int a_num_of_cards_front, int a_num_of_cards_back, int a_x, int a_y, float a_scale, int a_gap_of_prints)
: m_x(a_x)
, m_y(a_y)
, m_gap_of_prints(a_gap_of_prints)
, m_cards()
, m_back()
{
    m_cards.reserve(a_num_of_cards_front);
    m_back.reserve(a_num_of_cards_back);
    impl::init_back(m_back, a_num_of_cards_back, a_x, a_y, a_scale, a_gap_of_prints);
}

void Hand::push(std::string& a_suit, int a_number)
{
    std::string imagePath = "./resources/images/cards/" + a_suit + "-" + std::to_string(a_number) + ".png";
    m_cards.emplace_back(cardPointer(new Card(a_suit, a_number,imagePath)));
    impl::set_position(m_cards, m_x, m_y, m_gap_of_prints);
}

void Hand::push(std::vector<std::pair<std::string, int>> a_cards)
{
    std::string suit;
    int number;

    for(auto& pair : a_cards)
    {
        suit = pair.first;
        number = pair.second;
        push(suit, number);
    }
}

void Hand::clear()
{
    m_cards.clear();
}

void Hand::draw_front(sf::RenderWindow& a_window)const
{
    for(auto& card : m_cards)
    {
        card.get()->draw(a_window);
    }
}

void Hand::draw_back(sf::RenderWindow& a_window)const
{
    if(m_cards.empty())
        return;

    //const int size = m_cards.size();
    for(int i = 0; i < 2; ++i)
        m_back[i].get()->draw(a_window);
}

bool Hand::is_in_back_range(int a_x, int a_y)const
{
    if( a_x <= m_back[1].get()->right()  &&
        a_x >= m_back[0].get()->left()   &&
        a_y <= m_back[0].get()->bottom() &&
        a_y >= m_back[0].get()->top()   
    )return true;

    return false;
}


}// poker namespace