#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "shape.hpp"

namespace poker
{

class Chip : public Shape
{
public:
    Chip(int a_amount, std::string& a_image_path)
    : m_amount(a_amount)
    {
        m_texture.loadFromFile(a_image_path);
        m_shape.setTexture(m_texture);
        m_shape.scale(1, 1);
    };

    int amount() const
    {return m_amount;};
  
private:
    int m_amount;
};


}// poker namespace
