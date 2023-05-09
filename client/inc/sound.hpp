#pragma once

#include <fstream>
#include <SFML/Audio.hpp>

namespace poker
{

#define NUM_OF_SOUNDS 5

class Sound
{
public:
    Sound();

    void play_card();
    void play_chip();
    void play_button();
    void play_invalid();
    void play_positive();

private:
    std::vector<sf::Sound> m_sounds;
    std::vector<sf::SoundBuffer> m_sunds_buffers;
};


}// poker namespace