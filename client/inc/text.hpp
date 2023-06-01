#pragma once

#include <SFML/Graphics.hpp>

namespace poker
{

class Text
{
public:
    Text(std::string a_font_path, std::string a_text, sf::Color a_color, int a_size, int a_x, int a_y);

    void set_text(std::string a_text);
    void set_position(int a_x, int a_y);
    void set_style(sf::Text::Style& a_style);
    void set_font(std::string& a_font_path);
    void set_fill_color(sf::Color a_color);
    void set_outline_color(sf::Color& a_color);
    void set_outline_thickness(float a_thickness);

    void draw(sf::RenderWindow& a_window) const;

private:
    sf::Font m_font;
    sf::Text m_text;
};


}// poker namespace