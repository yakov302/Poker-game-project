#include "hand.hpp"

namespace poker
{

extern std::string empty;
CardsContainer cards_container;

namespace impl
{

void init_back(std::vector<cardPointer>& a_back, int& a_num_of_cards_back, int& a_x, int& a_y, float a_scale, int& a_gap_between_cards)
{
    int x = a_x - a_gap_between_cards;
    for(int i = 0; i < a_num_of_cards_back; ++i)
    {
        a_back.emplace_back(cardPointer(new Card(empty, 0, BACK_CARD_IMAGE_PATH)));
        a_back[i].get()->set_scale(a_scale, a_scale);
        a_back[i].get()->set_position(x += a_gap_between_cards + BACK_CARD_GAP, a_y + BACK_CARD_GAP);
    }
}

void set_position(std::vector<cardPointer>& a_cards, int& a_x, int& a_y, int& a_gap_between_cards)
{
    int x = a_x - a_gap_between_cards ;
    for(auto& card : a_cards)
       card.get()->set_position(x += a_gap_between_cards, a_y);
}


}//namespace impl

Hand::Hand(int a_num_of_cards_front, int a_num_of_cards_back, int a_x, int a_y, float a_scale, int a_gap_front, int a_gap_back)
: m_x(a_x)
, m_y(a_y)
, m_gap_between_cards(a_gap_front)
, m_mutex()
, m_cards()
, m_back()
{
    m_cards.reserve(a_num_of_cards_front);
    m_back.reserve(a_num_of_cards_back);
    impl::init_back(m_back, a_num_of_cards_back, a_x, a_y, a_scale, a_gap_back);
}

void Hand::push(std::string& a_suit, int a_number)
{
    m_cards.push_back(cards_container.give_card(a_suit, a_number));
    impl::set_position(m_cards, m_x, m_y, m_gap_between_cards);
}

void Hand::set_front_position(int a_x, int a_y)
{
    m_x = a_x;
    m_y = a_y;
}

void Hand::pop()
{
    Lock lock(m_mutex);
    if(m_cards.empty())[[unlikely]]
        return; 
    m_cards.pop_back();
}

void Hand::draw_front(sf::RenderWindow& a_window)const
{
    Lock lock(m_mutex);
    for(auto& card : m_cards)
        card.get()->draw(a_window);
}

void Hand::draw_back(sf::RenderWindow& a_window)const
{
    Lock lock(m_mutex);
    const int size = m_cards.size();
    for(int i = 0; i < size; ++i)
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

void Hand::clear()
{
    while(!m_cards.empty())
        m_cards.pop_back();
}


}// poker namespace
