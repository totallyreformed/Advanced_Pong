#include "Music.h"
#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>
#include "clamp.h"

Music::Music(GameState* gs, const std::string& name, const std::string& music_file,
    float volume, bool looping, bool isSoundEffect)
    : GameObject(gs, name),
    m_music_file(music_file),
    m_volume(volume),
    m_looping(looping),
    isSoundEffect(isSoundEffect)
{
}

void Music::init()
{
    std::string full_path = m_state->getFullAssetPath(m_music_file);

    if (!isSoundEffect)
    {
        // Attempt to play as background music
        graphics::playMusic(full_path, m_volume, m_looping);
        std::cout << "Music initialized: " << full_path << std::endl;
    }
    else
    {
        // Sound effect only loads; will be played on demand
        std::cout << "Sound effect initialized: " << full_path << std::endl;
    }
}

void Music::update(float dt)
{
    if (GameState::getInstance()->music_on) {
        play();
    }
    else {
        stop();
    }
}

void Music::draw()
{
    // No drawing needed for music
}

void Music::play()
{
    std::string full_path = m_state->getFullAssetPath(m_music_file);
    if (!isSoundEffect)
    {
        graphics::playMusic(full_path, m_volume, m_looping);
        std::cout << "Music playing: " << full_path << std::endl;
    }
    else
    {
        graphics::playSound(full_path, m_volume, false);
        std::cout << "Sound effect played: " << full_path << std::endl;
    }
}

void Music::stop()
{
    if (!isSoundEffect)
    {
        graphics::stopMusic();
        std::cout << "Music stopped.\n";
    }
    else
    {
        // Sound effects usually play once and can't be "stopped" easily
        std::cout << "Sound effect stopped (if applicable).\n";
    }
}

void Music::setVolume(float volume)
{
    m_volume = clamp(volume, 0.0f, 1.0f);

    if (!isSoundEffect)
    {
        // Restart background music with new volume
        graphics::stopMusic();
        std::string full_path = m_state->getFullAssetPath(m_music_file);
        graphics::playMusic(full_path, m_volume, m_looping);
        std::cout << "Music volume set to: " << m_volume << std::endl;
    }
    else
    {
        // For sound effects, volume is applied each time .play() is called
        std::cout << "Sound effect volume set to: " << m_volume << std::endl;
    }
}
