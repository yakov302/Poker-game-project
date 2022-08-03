#include "sound.hpp"

namespace poker
{

namespace impl
{

void sound_initialization(std::vector<sf::Sound>& a_sounds, std::vector<sf::SoundBuffer>& a_sunds_buffers)
{
    for(int i = 0; i < 6; i++)
        a_sunds_buffers.emplace_back(sf::SoundBuffer());

    a_sunds_buffers[0].loadFromFile("./resources/sounds/button.wav");
    a_sunds_buffers[1].loadFromFile("./resources/sounds/card.wav");
    a_sunds_buffers[2].loadFromFile("./resources/sounds/chip.wav");
    a_sunds_buffers[3].loadFromFile("./resources/sounds/positive.wav");
    a_sunds_buffers[4].loadFromFile("./resources/sounds/invalid.wav");


    for(int i = 0; i < 6; i++)
        a_sounds.emplace_back(sf::Sound());

    a_sounds[0].setBuffer(a_sunds_buffers[0]);
    a_sounds[1].setBuffer(a_sunds_buffers[1]);
    a_sounds[2].setBuffer(a_sunds_buffers[2]);
    a_sounds[3].setBuffer(a_sunds_buffers[3]);
    a_sounds[4].setBuffer(a_sunds_buffers[4]);
}


}// namespace impl

Sound::Sound()
: m_sounds()
, m_sunds_buffers()
{
    m_sounds.reserve(5);
    m_sunds_buffers.reserve(5);
    impl::sound_initialization(m_sounds, m_sunds_buffers);
}

void Sound::play_card()
{
    m_sounds[1].play();
}

void Sound::play_chip()
{
    m_sounds[2].play();
}

void Sound::play_positive()
{
    m_sounds[3].play();
}

void Sound::play_button()
{
    m_sounds[0].play();
}

void Sound::play_invalid()
{
    m_sounds[4].play();
}


}// poker namespace
