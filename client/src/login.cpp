#include "login.hpp"

namespace poker
{

namespace impl
{

static void buttons_initialization(std::unordered_map<std::string, buttonPointer>& a_buttons)
{
    a_buttons["background"] = buttonPointer(new Button("./resources/images/background/background.jpg", 0, 0 , 3));
    a_buttons["register"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 250, 0.5, "register"));
    a_buttons["log_in"] = buttonPointer(new Button("./resources/images/buttons/button.png", 1700, 310, 0.5, "log in"));
}

static void texts_initialization(std::unordered_map<std::string, textPointer>& a_texts)
{
    a_texts["duplcate"] = textPointer(new Text("./resources/fonts/arial.ttf", "duplcate", sf::Color(139, 134, 134), 25, 50, 100));
}

static void* thread_function(void* a_arg)
{
    Login* login = static_cast<Login*>(a_arg);
    login->run();
    return 0;
}


}//namespace impl

Login::Login(ActionOut& a_action_out)
: m_window(sf::VideoMode::getDesktopMode(), "Poker game")
, m_texts()
, m_buttons()
, m_action_out(a_action_out)
{
    impl::buttons_initialization(m_buttons);
    impl::texts_initialization(m_texts);
    m_thread = new std::thread(impl::thread_function, this);
}

Login::~Login()
{
    m_thread->join();
}


void Login::check_window()
{
    m_window.pollEvent(m_event);
    if(m_event.type == sf::Event::Closed)
        m_window.close();
}

void Login::draw_all()
{
    m_buttons["background"].get()->draw(m_window);

   
}


void Login::check_mouse()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {  

    }
}

void Login::run()
{
    while (m_window.isOpen())
    {
        draw_all();
        check_mouse();
        check_window();
        m_window.display();
    }
}


}// poker namespace
