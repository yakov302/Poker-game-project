#include "text.hpp"

namespace poker
{

Text::Text(std::string a_fontPath, std::string a_text, sf::Color a_color, int a_size, int a_x, int a_y)
: m_font()
, m_text(a_text, m_font, a_size)
{
    m_font.loadFromFile(a_fontPath);
    m_text.setPosition(a_x, a_y);
    m_text.setFillColor(a_color);
}

void Text::set_text(std::string a_text)
{
    m_text.setString(a_text);
}

void Text::set_position( int a_x, int a_y)
{
    m_text.setPosition(a_x, a_y);
}

void Text::set_style(sf::Text::Style& a_style)
{
    m_text.setStyle(a_style);
}

void Text::set_font(std::string& a_fontPath)
{
    m_font.loadFromFile(a_fontPath);
}

void Text::set_fill_color(sf::Color a_color)
{
    m_text.setFillColor(a_color);
}

void Text::set_outline_color(sf::Color& a_color)
{
    m_text.setOutlineColor(a_color);
}

void Text::set_outline_thickness(float a_thickness)
{
    m_text.setOutlineThickness(a_thickness);
}

void Text::draw(sf::RenderWindow& a_window)const
{
    a_window.draw(m_text);
}


}// poker namespace