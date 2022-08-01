#include "table.hpp"
#include "player.hpp"

#include <unistd.h>

namespace poker
{

namespace impl
{

void buttons_initialization(std::unordered_map<std::string, buttonPointer>& a_buttons)
{
    a_buttons["background"] = buttonPointer(new Button("./resources/images/background/background.jpg", 0, 0 , 3));
    a_buttons["go"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 250, 0.5, "go"));
    a_buttons["bet"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 310, 0.5, "bet"));
    a_buttons["check"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 370, 0.5, "check"));
    a_buttons["fold"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 430, 0.5, "fold"));
    a_buttons["exchange"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 490, 0.5, "exchange"));
    a_buttons["register"] = buttonPointer(new Button("./resources/images/buttons/button.png", 580, 370, 1, "register", 50, 22, 25));
    a_buttons["log_in"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1080, 370, 1, "log in", 20, 22, 25));
}

void texts_initialization(std::unordered_map<std::string, textPointer>& a_texts)
{
    a_texts["log_in"] = textPointer(new Text("./resources/fonts/arial.ttf", "", sf::Color(255, 228, 156), 30, 750, 80));
    a_texts["text"] = textPointer(new Text("./resources/fonts/arial.ttf", "", sf::Color(255, 228, 156), 30, 1570, 680));   
}

void texts_box_initialization(std::unordered_map<std::string, textBoxPointer>& a_text_boxs)
{
    a_text_boxs["name"] = textBoxPointer(new TextBox("./resources/images/text_box/text_box.png", 9, 835, 290, 1.2,"      name   ", -3, 22, 25));
    a_text_boxs["password"] = textBoxPointer(new TextBox("./resources/images/text_box/text_box.png", 9, 835, 370, 1.2, "   password  ", -3,  22, 25));
    a_text_boxs["gender"] = textBoxPointer(new TextBox("./resources/images/text_box/text_box.png", 9, 835, 450, 1.2, "     gender ", -4, 22, 25));
}

static void set_flags(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags["log_in"] = false;
    a_flags["register"] = false;
    a_flags["stop"] = false;
}

static void* thread_function(void* a_arg)
{
    Table* table = static_cast<Table*>(a_arg);
    table->run();
    return 0;
}


}//namespace impl

Table::Table(Hand& a_cards, Wallet& a_chips, Self& a_self, PlayersContainer& a_players, ActionOut& a_action_out)
: m_window(sf::VideoMode::getDesktopMode(), "Poker game")
, m_texts()
, m_buttons()
, m_text_boxs()
, m_cards(a_cards)
, m_chips(a_chips)
, m_self(a_self)
, m_players(a_players)
, m_action_out(a_action_out)
{
    impl::buttons_initialization(m_buttons);
    impl::texts_initialization(m_texts);
    impl::texts_box_initialization(m_text_boxs);
    impl::set_flags(m_flags);
    m_thread = new std::thread(impl::thread_function, this);
}

Table::~Table()
{
    m_thread->join();
}

void Table::stop()
{
    m_flags["stop"] = true;
    m_action_out.wake_up_client();
    m_window.close();
}

bool Table::is_runing()
{
    return !m_flags["stop"];
}

sf::RenderWindow& Table::window()
{
    return m_window;
}

void Table::check_events()
{
    m_window.pollEvent(m_event);

    switch (m_event.type)
    {
    case sf::Event::Closed:
        stop();
        break;
    
    case sf::Event::TextEntered:
        m_text_boxs["name"].get()->get_char(m_event);
        m_text_boxs["password"].get()->get_char(m_event);
        m_text_boxs["gender"].get()->get_char(m_event);
        usleep(70000);
        break;
    
    default:
        break;
    }
}

void Table::draw_login_screen()
{
    m_buttons["background"].get()->draw(m_window);
    m_texts["log_in"].get()->draw(m_window);

    if(m_flags["log_in"])
    {
        draw_log_in();
        return;
    }

    if(m_flags["register"])
    {   
        draw_register();
        return;
    }

    m_buttons["register"].get()->draw(m_window); 
    m_buttons["log_in"].get()->draw(m_window); 
}

void Table::draw_log_in()
{
    m_text_boxs["name"].get()->draw(m_window);
    m_text_boxs["password"].get()->draw(m_window);
    m_buttons["log_in"].get()->draw(m_window); 
}

void Table::draw_register()
{
    m_text_boxs["name"].get()->draw(m_window);
    m_text_boxs["password"].get()->draw(m_window);
    m_text_boxs["gender"].get()->draw(m_window);
    m_buttons["register"].get()->draw(m_window); 
}

void Table::draw_all()
{
    m_buttons["background"].get()->draw(m_window);
    m_texts["text"].get()->draw(m_window);

    m_players.draw_Players(m_window);

    m_cards.draw_front(m_window);
    m_chips.draw(m_window, 700, 473, -1);
    m_chips.print_amount(m_window);

    m_buttons["go"].get()->draw(m_window) ;
    m_buttons["exchange"].get()->draw(m_window);

    if(m_self.is_flag_on("my_turn"))
    {
        draw_your_turn();
    }
}

void Table::draw_your_turn()
{
    m_buttons["bet"].get()->draw(m_window); 
    m_buttons["check"].get()->draw(m_window); 
    m_buttons["fold"].get()->draw(m_window); 
}

void Table::check_mouse_looged()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {    
        if(check_reveal_cards())
            return;

        if(check_go_button())
            return;

        if(check_exchange_button())
            return;

        if(m_self.is_flag_on("my_turn"))
        {
            if(check_your_turn())
                return;
        }
    }
}

bool Table::check_your_turn()
{
    if(check_bet_button())
        return true;

    if(check_check_button())
        return true;

    if(check_fold_button())
        return true;
    
    return false;
}

bool Table::check_go_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if (m_buttons["go"].get()->is_in_range(position.x, position.y))
    {
        if(m_self.is_flag_on("bet"))
        {
            set_text("text", "");
            m_action_out.finish_bet(m_self.name());
            usleep(100000);
        }

        if(m_self.is_flag_on("exchange"))
        {
            m_self.turn_off_flag("exchange");
            m_self.set_action("");
        }

        return true;
    }

    return false;
}


bool Table::check_bet_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if (m_buttons["bet"].get()->is_in_range(position.x, position.y) && !m_self.is_flag_on("exchange"))
    {
        m_action_out.start_bet(m_self.name());
        return true;
    }

    if(m_self.is_in_wallet_range(position.x, position.y) && m_self.is_flag_on("bet"))
    {
        int chip = m_self.bet(position.x, position.y);
        m_action_out.bet_action(m_self.name(), chip);
        usleep(100000);
        return true;
    }

    return false;
}

bool Table::check_check_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if (m_buttons["check"].get()->is_in_range(position.x, position.y))
    {
        m_action_out.check_action();
        usleep(100000);
        return true;
    }

    return false;
}

bool Table::check_fold_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if (m_buttons["fold"].get()->is_in_range(position.x, position.y))
    {
        m_action_out.fold_action();
        return true;
    }

    return false;
}

bool Table::check_exchange_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_buttons["exchange"].get()->is_in_range(position.x, position.y) && !m_self.is_flag_on("bet"))
    {
        m_self.turn_on_flag("exchange");
        m_self.set_action("exchange");
        return true;
    }

    if(m_self.is_flag_on("exchange") && m_self.is_in_wallet_range(position.x, position.y))
    {
        m_self.exchange(position.x, position.y);
        usleep(100000);
        return true;
    }

    return false;
}

bool Table::check_reveal_cards()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_self.is_in_back_range(position.x, position.y))
    {
        m_self.draw_hand_front(m_window);
        return true;
    }

    return false;
}

void Table::check_mouse_not_looged()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {  
        if(m_flags["log_in"])
        {
            if(check_name_bar())
                return;
            if(check_password_bar())
                return;
            if(check_log_in_button(true))
                return;
        }
        else if(!m_flags["log_in"])
        {
            if(check_log_in_button(false))
                return;
        }

        if(m_flags["register"])
        {
            if(check_name_bar())
                return; 
            if(check_password_bar())
                return;
            if(check_gender_bar())
                return;
            if(check_register_button(true))
                return;
        }
        else if(!m_flags["register"])
        {
            check_register_button(false);
        }
    }
}

bool Table::check_name_bar()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_text_boxs["name"].get()->is_in_range(position.x, position.y))
    {
        m_text_boxs["name"].get()->set_select(true);
        return true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_text_boxs["name"].get()->is_in_range(position.x, position.y) && m_text_boxs["name"].get()->is_select())
    {
        m_text_boxs["name"].get()->set_select(false);
        return true;
    }

    return false;
}

bool Table::check_password_bar()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_text_boxs["password"].get()->is_in_range(position.x, position.y))
    {
        m_text_boxs["password"].get()->set_select(true);
        return true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_text_boxs["password"].get()->is_in_range(position.x, position.y) && m_text_boxs["password"].get()->is_select())
    {
        m_text_boxs["password"].get()->set_select(false);
        return true;
    }
    return false;
}

bool Table::check_gender_bar()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_text_boxs["gender"].get()->is_in_range(position.x, position.y))
    {
        m_text_boxs["gender"].get()->set_select(true);
        return true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_text_boxs["gender"].get()->is_in_range(position.x, position.y) && m_text_boxs["gender"].get()->is_select())
    {
        m_text_boxs["gender"].get()->set_select(false);
        return true;
    }
    return false;
}

bool Table::check_log_in_button(bool a_is_on)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(!a_is_on)
    {
        if(m_buttons["log_in"].get()->is_in_range(position.x, position.y))
        {
            m_flags["log_in"] = true;
            usleep(100000);
            return true;
        }
    }
    else
    {
        if(m_buttons["log_in"].get()->is_in_range(position.x, position.y))
        {
            m_action_out.log_in_request(m_text_boxs["name"].get()->give_string(), m_text_boxs["password"].get()->give_string(), m_self.amount());
            m_text_boxs["name"].get()->clear();
            m_text_boxs["password"].get()->clear();
            m_texts["log_in"].get()->set_text("         Request sent");
            usleep(100000);
            return true;
        }
    }
    return false;
}

bool Table::check_register_button(bool a_is_on)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(!a_is_on)
    {
        if(m_buttons["register"].get()->is_in_range(position.x, position.y))
        {
            m_flags["register"] = true;
            usleep(100000);
            return true;
        }
    }
    else
    {
        if(m_buttons["register"].get()->is_in_range(position.x, position.y))
        {
            m_action_out.registration_request(m_text_boxs["name"].get()->give_string(), m_text_boxs["password"].get()->give_string(), m_text_boxs["gender"].get()->give_string());
            m_text_boxs["name"].get()->clear();
            m_text_boxs["password"].get()->clear();
            m_text_boxs["gender"].get()->clear();
            m_texts["log_in"].get()->set_text("         Request sent");
            usleep(100000);
            return true;
        }
    }
    return false;
}

void Table::run()
{
    run_log_in();

    while (m_window.isOpen() && is_runing())
    {
        m_window.clear();
        draw_all();
        check_mouse_looged();
        check_events();
        m_window.display();
    }
}

void Table::run_log_in()
{
    while (m_window.isOpen() && !m_self.is_flag_on("logged") && is_runing())
    {
        m_window.clear();
        draw_login_screen();
        check_mouse_not_looged();
        check_events();
        m_window.display();
    }
}

void Table::turn_on_flag(std::string a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())
        return;

    m_flags[a_flag] = true;
}

void Table::turn_off_flag(std::string a_flag)
{
    if(m_flags.find(a_flag) == m_flags.end())
        return;

    m_flags[a_flag] = false;
}

void Table::set_text(std::string a_type, std::string a_text, sf::Color a_color)
{
    m_texts[a_type].get()->set_text(a_text);
    m_texts[a_type].get()->set_fill_color(a_color);
}

}// poker namespace
