#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

namespace poker
{

class Shape
{
public:
    void set_position(int a_x, int a_y)
    {
        m_shape.setPosition(a_x, a_y);
    };

    void set_scale(float a_width, float a_height)
    {
        m_shape.scale(a_height, a_width);
    };

    void draw(sf::RenderWindow& a_window)const
    {
        a_window.draw(m_shape);
    }; 

    int right()const
    {
        return m_shape.getPosition().x + m_shape.getGlobalBounds().width;
    };

    int left()const
    {
        return m_shape.getPosition().x;
    };

    int top()const
    {
        return m_shape.getPosition().y;
    };

    int bottom()const
    {
        return m_shape.getPosition().y + m_shape.getGlobalBounds().height;
    }

    int width()const
    {
        return m_shape.getGlobalBounds().width;
    }

    int height()const
    {
        return m_shape.getGlobalBounds().height;
    }

    sf::Sprite m_shape;
    sf::Texture m_texture;
};


}// poker namespace