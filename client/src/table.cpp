#include "table.hpp"

namespace poker
{

extern Sound sound;
std::string empty = "";
std::string request_sent = "Request sent";

namespace impl
{

extern int log_in_text_x_pos(std::string& txt);

void buttons_initialization(std::unordered_map<std::string, buttonPointer>& a_buttons) 
{
    a_buttons["go"]         = buttonPointer(new Button(BUTTON_IMAGE_PATH,     BUTTON_X_POS, BUTTON_GAME_Y_POS + 0*BUTTON_SIZE, BUTTON_GAME_IMAGE_SCALE,        "go",       TEXT_BUTTON_X_GAP,  GAME_TEXT_BUTTON_Y_GAP, GAME_TEXT_SZIE       ));
    a_buttons["bet"]        = buttonPointer(new Button(BUTTON_IMAGE_PATH,     BUTTON_X_POS, BUTTON_GAME_Y_POS + 1*BUTTON_SIZE, BUTTON_GAME_IMAGE_SCALE,        "bet",      TEXT_BUTTON_X_GAP,  GAME_TEXT_BUTTON_Y_GAP, GAME_TEXT_SZIE       ));
    a_buttons["fold"]       = buttonPointer(new Button(BUTTON_IMAGE_PATH,     BUTTON_X_POS, BUTTON_GAME_Y_POS + 3*BUTTON_SIZE, BUTTON_GAME_IMAGE_SCALE,        "fold",     TEXT_BUTTON_X_GAP,  GAME_TEXT_BUTTON_Y_GAP, GAME_TEXT_SZIE       ));
    a_buttons["check"]      = buttonPointer(new Button(BUTTON_IMAGE_PATH,     BUTTON_X_POS, BUTTON_GAME_Y_POS + 2*BUTTON_SIZE, BUTTON_GAME_IMAGE_SCALE,        "check",    TEXT_BUTTON_X_GAP,  GAME_TEXT_BUTTON_Y_GAP, GAME_TEXT_SZIE       ));
    a_buttons["exchange"]   = buttonPointer(new Button(BUTTON_IMAGE_PATH,     BUTTON_X_POS, BUTTON_GAME_Y_POS + 4*BUTTON_SIZE, BUTTON_GAME_IMAGE_SCALE,        "exchange", TEXT_BUTTON_X_GAP,  GAME_TEXT_BUTTON_Y_GAP, GAME_TEXT_SZIE       ));
    a_buttons["log_in"]     = buttonPointer(new Button(BUTTON_IMAGE_PATH,     1080,         BUTTON_OPEN_SCREEN_Y_POS,          BUTTON_OPEN_SCREEN_IMAGE_SCALE, "log in",   20,                 OPEN_SCREEN_TEXT_Y_GAP, OPEN_SCREEN_TEXT_SZIE));
    a_buttons["register"]   = buttonPointer(new Button(BUTTON_IMAGE_PATH,     580,          BUTTON_OPEN_SCREEN_Y_POS,          BUTTON_OPEN_SCREEN_IMAGE_SCALE, "register", 50,                 OPEN_SCREEN_TEXT_Y_GAP, OPEN_SCREEN_TEXT_SZIE));
    a_buttons["background"] = buttonPointer(new Button(BACKGTOUND_IMAGE_PATH, ZERO_POS,     ZERO_POS,                          BACKGTOUND_IMAGE_SCALE,         "",         TEXT_BUTTON_X_GAP,  GAME_TEXT_BUTTON_Y_GAP, GAME_TEXT_SZIE       ));
}

void texts_box_initialization(std::unordered_map<std::string, textBoxPointer>& a_text_boxs)
{
    a_text_boxs["name"]     = textBoxPointer(new TextBox(TEXT_BOX_IMAGE_PATH, TEXT_BOX_DIGITS_LIMET, TEXT_BOX_X_POS, TEXT_BOX_Y_POS + 0*TEXT_BOX_SIZE, TEXT_BOX_IMAGE_SCALE, "      name   ", -3, OPEN_SCREEN_TEXT_Y_GAP, OPEN_SCREEN_TEXT_SZIE));
    a_text_boxs["gender"]   = textBoxPointer(new TextBox(TEXT_BOX_IMAGE_PATH, TEXT_BOX_DIGITS_LIMET, TEXT_BOX_X_POS, TEXT_BOX_Y_POS + 2*TEXT_BOX_SIZE, TEXT_BOX_IMAGE_SCALE, "     gender ",  -4, OPEN_SCREEN_TEXT_Y_GAP, OPEN_SCREEN_TEXT_SZIE));
    a_text_boxs["password"] = textBoxPointer(new TextBox(TEXT_BOX_IMAGE_PATH, TEXT_BOX_DIGITS_LIMET, TEXT_BOX_X_POS, TEXT_BOX_Y_POS + 1*TEXT_BOX_SIZE, TEXT_BOX_IMAGE_SCALE, "   password  ", -3, OPEN_SCREEN_TEXT_Y_GAP, OPEN_SCREEN_TEXT_SZIE));
}

static void flags_initialization(std::unordered_map<std::string, bool>& a_flags)
{
    a_flags["stop"]     = false;
    a_flags["log_in"]   = false;
    a_flags["register"] = false;
}

static void* thread_function(void* a_arg)
{
    Table* table = static_cast<Table*>(a_arg);
    table->run();
    return 0;
}


}//namespace impl

Table::Table(Hand& a_cards, Wallet& a_chips, Self& a_self, PlayersContainer& a_players, ActionOut& a_action_out)
: m_window(sf::VideoMode::getDesktopMode(), "My Texas Hold'em")
, m_text(new Text(ARIAL_FONT_PATH, "", TEXT_COLOR, TABLE_TEXT_SIZE, 0, 0))
, m_buttons()
, m_text_boxs()
, m_self(a_self)
, m_cards(a_cards)
, m_chips(a_chips)
, m_action_out(a_action_out)
, m_players(a_players)
{
    impl::buttons_initialization(m_buttons);
    impl::texts_box_initialization(m_text_boxs);
    impl::flags_initialization(m_flags);
    m_thread = new std::thread(impl::thread_function, this);
}

Table::~Table()
{
    m_thread->join();
    delete m_thread;
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
        break;
    
    default:
        break;
    }
}

void Table::draw_login_screen()
{
    m_buttons["background"].get()->draw(m_window);
    m_text.get()->draw(m_window);

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
    m_text.get()->draw(m_window);

    m_players.draw_Players(m_window);

    m_cards.draw_front(m_window);
    m_chips.draw(m_window, true);

    m_buttons["go"].get()->draw(m_window) ;
    m_buttons["exchange"].get()->draw(m_window);

    if(m_self.is_flag_on("my_turn"))
        draw_your_turn();
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
        sound.play_button();
        usleep(100000);

        if(m_self.is_flag_on("exchange"))
        {
            m_self.turn_off_flag("exchange");
            m_self.set_action("");
            if(m_self.is_flag_on("bet"))
                m_action_out.start_bet(m_self.name());

            return true;
        }

        if(m_self.is_flag_on("bet"))
        {
            set_text(empty, 0, 0);
            m_action_out.finish_bet(m_self.name());
            return true;
        }
    }

    return false;
}

bool Table::check_bet_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if (m_buttons["bet"].get()->is_in_range(position.x, position.y) && !m_self.is_flag_on("exchange"))
    {
        sound.play_button();
        m_action_out.start_bet(m_self.name());
        usleep(100000);
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
        sound.play_button();
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
        sound.play_button();
        m_action_out.fold_action();
        usleep(100000);
        return true;
    }

    return false;
}

bool Table::check_exchange_button()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_buttons["exchange"].get()->is_in_range(position.x, position.y))
    {
        sound.play_button();
        m_self.turn_on_flag("exchange");
        m_self.set_action("exchange");
        usleep(100000);
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

void Table::turn_off_all_bars()
{
    for(auto& bar : m_text_boxs)
        bar.second.get()->set_select(false);
}

bool Table::check_name_bar()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f position = m_window.mapPixelToCoords(pixelPos);

    if(m_text_boxs["name"].get()->is_in_range(position.x, position.y))
    {
        turn_off_all_bars();
        m_text_boxs["name"].get()->set_select(true);
        return true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
       !m_text_boxs["name"].get()->is_in_range(position.x, position.y) && 
       m_text_boxs["name"].get()->is_select())
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
        turn_off_all_bars();
        m_text_boxs["password"].get()->set_select(true);
        return true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
      !m_text_boxs["password"].get()->is_in_range(position.x, position.y) && 
      m_text_boxs["password"].get()->is_select())
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
        turn_off_all_bars();
        m_text_boxs["gender"].get()->set_select(true);
        return true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
       !m_text_boxs["gender"].get()->is_in_range(position.x, position.y) && 
       m_text_boxs["gender"].get()->is_select())
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
            sound.play_button();
            m_flags["log_in"] = true;
            usleep(100000);
            return true;
        }
    }
    else
    {
        if(m_buttons["log_in"].get()->is_in_range(position.x, position.y))
        {
            sound.play_button();
            m_action_out.log_in_request(m_text_boxs["name"].get()->give_string(), m_text_boxs["password"].get()->give_string(), m_self.amount());
            m_text_boxs["name"].get()->clear();
            m_text_boxs["password"].get()->clear();
            set_text(request_sent, impl::log_in_text_x_pos(request_sent), LOG_IN_TEXT_Y_POS);
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
            sound.play_button();
            m_flags["register"] = true;
            usleep(100000);
            return true;
        }
    }
    else
    {
        if(m_buttons["register"].get()->is_in_range(position.x, position.y))
        {
            sound.play_button();
            m_action_out.registration_request(m_text_boxs["name"].get()->give_string(), m_text_boxs["password"].get()->give_string(), m_text_boxs["gender"].get()->give_string());
            m_text_boxs["name"].get()->clear();
            m_text_boxs["password"].get()->clear();
            m_text_boxs["gender"].get()->clear();
            set_text(request_sent, impl::log_in_text_x_pos(request_sent), LOG_IN_TEXT_Y_POS);
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

void Table::set_text(std::string& a_text, int x_pos, int y_pos)
{
    m_text.get()->set_text(a_text);
    m_text.get()->set_position(x_pos, y_pos);
}


}// poker namespace
