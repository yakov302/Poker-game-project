#pragma once

#include <vector>

#include "card.hpp"

namespace poker
{

using cardPointer = std::shared_ptr<Card>;

class Player
{
public:
    Player(std::string a_name, std::string a_gender, int a_amount, int a_socket)
    : m_name(a_name)
    , m_gender(a_gender)
    , m_amount(a_amount)
    , m_socket(a_socket)
    , m_result(0)
    , m_bet(0)
    , m_fold(false)
    , m_viewer(false)
    , m_my_turn(false)
    , m_hand()
    {
        m_hand.reserve(2);
    };

    std::string m_name;
    std::string m_gender;

    int m_amount;
    int m_socket;
    int m_result;
    int m_bet;

    bool m_fold;
    bool m_viewer;
    bool m_my_turn;
    
    std::vector<cardPointer> m_hand;
};


}// poker namespace
