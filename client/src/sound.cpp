#include "sound.hpp"

namespace poker
{

namespace impl
{

void sound_initialization(std::vector<sf::Sound>& a_sounds, std::vector<sf::SoundBuffer>& a_sunds_buffers)
{
    for(int i = 0; i < 7; i++)
        a_sunds_buffers.emplace_back(sf::SoundBuffer());

    a_sunds_buffers[0].loadFromFile("./resources/sounds/button.wav");
    a_sunds_buffers[1].loadFromFile("./resources/sounds/card.wav");
    a_sunds_buffers[2].loadFromFile("./resources/sounds/chip_in.wav");
    a_sunds_buffers[3].loadFromFile("./resources/sounds/chip_out.wav");
    a_sunds_buffers[4].loadFromFile("./resources/sounds/invalid.wav");
    a_sunds_buffers[5].loadFromFile("./resources/sounds/lose.wav");
    a_sunds_buffers[6].loadFromFile("./resources/sounds/win.wav");

    for(int i = 0; i < 7; i++)
        a_sounds.emplace_back(sf::Sound());

    a_sounds[0].setBuffer(a_sunds_buffers[0]);
    a_sounds[1].setBuffer(a_sunds_buffers[1]);
    a_sounds[2].setBuffer(a_sunds_buffers[2]);
    a_sounds[3].setBuffer(a_sunds_buffers[3]);
    a_sounds[4].setBuffer(a_sunds_buffers[4]);
    a_sounds[5].setBuffer(a_sunds_buffers[5]);
    a_sounds[6].setBuffer(a_sunds_buffers[6]);
}


}// namespace impl

Sound::Sound()
: m_sounds()
, m_sunds_buffers()
{
    m_sounds.reserve(7);
    m_sunds_buffers.reserve(7);
    impl::sound_initialization(m_sounds, m_sunds_buffers);
}

void Sound::play_card()
{
    m_sounds[1].play();
}

void Sound::play_chip_in()
{
    m_sounds[2].play();
}

void Sound::play_chip_out()
{
    m_sounds[2].play();
}

void Sound::play_win()
{
    m_sounds[6].play();
}

void Sound::play_lose()
{
    m_sounds[5].play();
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
