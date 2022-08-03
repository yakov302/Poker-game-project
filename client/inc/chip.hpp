#pragma once

#include "shape.hpp"

namespace poker
{

class Chip : public Shape
{
public:
    Chip(int a_amount, std::string& a_image_path, float a_scale = 1)
    : m_amount(a_amount)
    {
        m_texture.loadFromFile(a_image_path);
        m_shape.setTexture(m_texture);
        m_shape.scale(a_scale, a_scale);
    };

    int amount() const
    {return m_amount;};
  
private:
    int m_amount;
};


}// poker namespace
