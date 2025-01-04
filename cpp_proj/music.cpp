#include "Music.h"
#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>
#include "clamp.h"

/**
 * @brief Constructs a new Music object.
 *
 * Initializes the Music object with the specified parameters. Determines whether the
 * music file is a background track or a sound effect and sets initial properties accordingly.
 *
 * @param gs Pointer to the current GameState.
 * @param name The name identifier for the Music object.
 * @param music_file The filename of the music or sound effect.
 * @param volume The playback volume (0.0f to 1.0f).
 * @param looping Whether the music should loop continuously.
 * @param isSoundEffect Flag indicating if the file is a sound effect.
 */
Music::Music(GameState* gs, const std::string& name, const std::string& music_file,
    float volume, bool looping, bool isSoundEffect)
    : GameObject(gs, name),
    m_music_file(music_file),
    m_volume(volume),
    m_looping(looping),
    isSoundEffect(isSoundEffect)
{
    // Constructor body can be expanded if additional initialization is required
}

/**
 * @brief Initializes the Music object.
 *
 * Loads and starts playing the music or prepares the sound effect for playback.
 * For background music, it begins playback immediately if not a sound effect.
 * Sound effects are loaded and can be played on demand.
 */
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

/**
 * @brief Updates the Music object's state.
 *
 * Checks the global music settings and plays or stops the music accordingly.
 * Ensures that background music respects the game's music toggle state.
 *
 * @param dt Delta time since the last update in seconds.
 */
void Music::update(float dt)
{
    if (GameState::getInstance()->music_on) {
        play();
    }
    else {
        stop();
    }
}

/**
 * @brief Renders the Music object.
 *
 * No rendering is required for music objects as they do not have a visual representation.
 * This method is intentionally left empty.
 */
void Music::draw()
{
    // No drawing needed for music
}

/**
 * @brief Plays the music or sound effect.
 *
 * For background music, it starts playing the track with the specified volume and looping settings.
 * For sound effects, it plays the sound once at the current volume.
 */
void Music::play()
{
    std::string full_path = m_state->getFullAssetPath(m_music_file);
    if (!isSoundEffect)
    {
        // Play background music with current settings
        graphics::playMusic(full_path, m_volume, m_looping);
        std::cout << "Music playing: " << full_path << std::endl;
    }
    else
    {
        // Play sound effect once
        graphics::playSound(full_path, m_volume, false);
        std::cout << "Sound effect played: " << full_path << std::endl;
    }
}

/**
 * @brief Stops the music or sound effect.
 *
 * For background music, it stops playback. Sound effects typically play once and do not need to be stopped.
 */
void Music::stop()
{
    if (!isSoundEffect)
    {
        // Stop background music
        graphics::stopMusic();
        std::cout << "Music stopped.\n";
    }
    else
    {
        // Sound effects usually play once and can't be "stopped" easily
        std::cout << "Sound effect stopped (if applicable).\n";
    }
}

/**
 * @brief Sets the playback volume for the music or sound effect.
 *
 * Adjusts the volume while ensuring it remains within the valid range [0.0f, 1.0f].
 * For background music, it restarts the track with the new volume.
 * For sound effects, the new volume is applied each time the sound is played.
 *
 * @param volume The desired volume level (0.0f to 1.0f).
 */
void Music::setVolume(float volume)
{
    // Ensure the volume stays within [0.0f, 1.0f]
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
