#pragma once

#include "shape.hpp"

namespace poker
{

class Card : public Shape
{
public:
    Card(std::string& a_suit, int a_number, std::string a_image_path, float a_scale = 0.7)
    : m_suit(a_suit)
    , m_number(a_number)
    {
        m_texture.loadFromFile(a_image_path);
        m_shape.setTexture(m_texture);
        m_shape.scale(a_scale, a_scale);
    };

    std::string& suit() const
    {return m_suit;};

    int number() const
    {return m_number;};

private:
    std::string& m_suit;
    int m_number;
};


}// poker namespace
