#pragma once

#include "button.hpp"

#define DELETE 8
#define TYPING_SPEED 14
#define LEGAL_UNICODE 128
#define DEFAULT_UNICODE 255

namespace poker
{

class TextBox : public Button
{
public:
    TextBox(std::string a_image_path, int a_limet, int a_x, int a_y, float a_scale, std::string a_text, int a_x_gap, int a_y_gap, int text_size);

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
    int m_lest_unicode_counter;
    int m_lest_unicode;
    bool m_is_select;
    int m_limet;
};


}// poker namespace
