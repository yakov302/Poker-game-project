#pragma once

#include <SFML/Audio.hpp>
 #include <unistd.h>

namespace poker
{

class Sound
{
public:
    Sound();

    void play_card();
    void play_chip();
    void play_win();
    void play_button();
    void play_invalid();
    void play_positive();

private:
    std::vector<sf::Sound> m_sounds;
    std::vector<sf::SoundBuffer> m_sunds_buffers;
};


}// poker namespace