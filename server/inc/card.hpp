#pragma once

#include <string>

namespace poker
{

class Card
{
public:
    Card(std::string a_suit, int a_number)
    : m_suit(a_suit)
    , m_number(a_number)
    {};
    
    std::string m_suit;
    int m_number;
};


}//namespace poker
