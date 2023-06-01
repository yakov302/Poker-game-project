#pragma once

#include "shape.hpp"
#include "text.hpp"

#define MAX_TEXT_SIZE 9
#define TEXT_COLOR sf::Color(255, 228, 156)
#define ARIAL_FONT_PATH "./resources/fonts/arial.ttf"

namespace poker
{

class Button : public Shape
{
public:
    Button(std::string a_image_path, int a_x, int a_y, float a_scale, std::string a_text, int a_text_x_gap, int a_text_y_gap, int a_text_size)
    : m_text(ARIAL_FONT_PATH, a_text, TEXT_COLOR, a_text_size, a_x + (MAX_TEXT_SIZE - a_text.size())*a_text_x_gap, a_y + a_text_y_gap)
    {
        m_texture.loadFromFile(a_image_path);
        m_shape.setTexture(m_texture);
        m_shape.scale(a_scale, a_scale);
        m_shape.setPosition(a_x, a_y);
    };

    virtual ~Button(){};

    bool is_in_range(int a_x, int a_y)
    {
        if( a_x <= this->right()  &&
            a_x >= this->left()   &&
            a_y <= this->bottom() &&
            a_y >= this->top()    
        )return true;

        return false;
    };

    void draw(sf::RenderWindow& a_window)const
    {
        a_window.draw(m_shape);
        m_text.draw(a_window);
    }; 

protected:
    Text m_text;
};


}// poker namespace
