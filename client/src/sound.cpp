#include "sound.hpp"

namespace poker
{

namespace impl
{

void sound_initialization(std::vector<sf::Sound>& a_sounds, std::vector<sf::SoundBuffer>& a_sunds_buffers, std::ifstream& a_config_file)
{
    int i = 0;
    std::string sound_file_name;    
    while(std::getline(a_config_file, sound_file_name))
    {   
        a_sunds_buffers.emplace_back(sf::SoundBuffer());
        a_sunds_buffers[i].loadFromFile(sound_file_name);
        a_sounds.emplace_back(sf::Sound());
        a_sounds[i].setBuffer(a_sunds_buffers[i]);
        ++i;    
    }
}


}// namespace impl

Sound::Sound()
: m_sounds()
, m_sunds_buffers()
{
    m_sounds.reserve(NUM_OF_SOUNDS);
    m_sunds_buffers.reserve(NUM_OF_SOUNDS);
    std::ifstream sound_config_file("./resources/txt/sounds_config.txt");
    impl::sound_initialization(m_sounds, m_sunds_buffers, sound_config_file);
}

void Sound::play_card()
{
    m_sounds[0].play();
}

void Sound::play_chip()
{
    m_sounds[1].play();
}

void Sound::play_button()
{
    m_sounds[2].play();
}

void Sound::play_invalid()
{
    m_sounds[3].play();
}

void Sound::play_positive()
{
    m_sounds[4].play();
}

}// poker namespace
