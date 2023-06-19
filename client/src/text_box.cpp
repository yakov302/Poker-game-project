#include "text_box.hpp"

namespace poker
{

TextBox::TextBox(std::string a_image_path, int a_limet, int a_x , int a_y, float a_scale, std::string a_text, int a_x_gap, int a_y_gap, int a_text_size)
: Button(a_image_path, a_x, a_y, a_scale, a_text , a_x_gap, a_y_gap, a_text_size)
, m_background_string(a_text)
, m_string()
, m_lest_unicode_counter(0)
, m_lest_unicode(DEFAULT_UNICODE)
, m_is_select(false)
, m_limet(a_limet)
{
    m_text.set_fill_color(sf::Color(183, 218, 191));
}

void TextBox::get_char(sf::Event a_typing)
{
    if(!m_is_select)
        return;

    int typing = a_typing.text.unicode;
    if(typing > LEGAL_UNICODE)[[unlikely]]
        return;

    if(typing == m_lest_unicode)
    {
        ++m_lest_unicode_counter;
        if(m_lest_unicode_counter < TYPING_SPEED)
            return;
    }

    m_lest_unicode_counter = 0;
    m_lest_unicode = typing;

    if(static_cast<int>(m_string.size()) < m_limet)
        input(typing);
    else if(typing == DELETE)
        delete_char();    
}

std::string TextBox::give_string()const
{
    return m_string;
}

bool TextBox::is_select()const
{
    return m_is_select;
}

void TextBox::set_select(bool a_choice)
{
    m_is_select = a_choice;
    if(a_choice == true)
    {
        m_text.set_fill_color(sf::Color(255, 228, 156));
        m_text.set_text("_");
        m_string.clear();
    }
    if(a_choice == false)
        m_text.set_text(m_string);

}

void TextBox::clear()
{
    m_text.set_text(m_background_string);
    m_text.set_fill_color(sf::Color(183, 218, 191));
    m_string.clear();
}

void TextBox::input(int a_char)
{
    if(a_char != DELETE)
        m_string += static_cast<char>(a_char);
    else
    {
        if(m_string.size() > 0)
            delete_char();
    }

    m_text.set_text(m_string + "_");
}

void TextBox::delete_char()
{
    m_string.resize( m_string.size()-1);
}


}// poker namespace
