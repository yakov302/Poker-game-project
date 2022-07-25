#pragma once

#include <string>
#include <vector>

#include "card.hpp"

namespace poker
{

class Player
{
public:
    Player(std::string a_name, std::string a_gender, int a_amount, int a_socket)
    : m_name(a_name)
    , m_gender(a_gender)
    , m_amount(a_amount)
    , m_socket(a_socket)
    , m_fold(false)
    , m_my_turn(false)
    , m_hand()
    {
        m_hand.reserve(2);
    };

    std::string m_name;
    std::string m_gender;
    int m_amount;
    int m_socket;
    bool m_fold;
    bool m_my_turn;
    std::vector<Card> m_hand;
};


}// poker namespace
