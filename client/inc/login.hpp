#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <thread>

#include "action_out.hpp"
#include "button.hpp"
#include "text.hpp"

namespace poker
{

using buttonPointer = std::shared_ptr<Button>;
using textPointer = std::shared_ptr<Text>;

class Login
{
public:
    Login(ActionOut& a_action_out);
    ~Login();
    
    void run();
    void stop();

private:
    void check_window();
    void draw_all();
    void check_mouse();
   
private:
    sf::Event m_event;
    sf::RenderWindow m_window;
    std::thread* m_thread;
    std::unordered_map<std::string, textPointer> m_texts;
    std::unordered_map<std::string, buttonPointer> m_buttons;

    ActionOut& m_action_out;
};


}//namespace poker
