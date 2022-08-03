#pragma once

#include "button.hpp"

#define DELETE 8

namespace poker
{

class TextBox : public Button
{
public:
    TextBox(std::string a_image_path, int a_limet, int a_x = 0, int a_y = 0, float a_scale = 1, std::string a_text = "", int a_x_gap = 5, int a_y_gap = 8, int text_size = 20);

    void get_char(sf::Event a_typing);
    void set_select(bool a_choice);
    void clear();

    std::string give_string() const;
    bool is_select()const;

private:
    void input(int a_char);
    void delete_char();

private:
    std::string m_background_string;
    std::string m_string;
    bool m_is_select;
    int m_limet;
};


}// poker namespace
