#pragma once

#include "shape.hpp"

namespace poker
{

class Chip : public Shape
{
public:
    Chip(int a_amount, std::string& a_image_path, int a_index_id = 0, float a_scale = 1)
    : m_amount(a_amount)
    , m_index_id(a_index_id)
    {
        m_texture.loadFromFile(a_image_path);
        m_shape.setTexture(m_texture);
        m_shape.scale(a_scale, a_scale);
    };

    void set_index_id(int a_index_id)
    {m_index_id = a_index_id;}

    int index_id() const
    {return m_index_id;}

    int amount() const
    {return m_amount;};
  
private:
    int m_amount;
    int m_index_id;
};


}// poker namespace
