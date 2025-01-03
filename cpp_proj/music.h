#pragma once
#include "GameObject.h"
#include <string>

/**
 * @class Music
 * @brief Manages background music or sound effects in the game.
 */
class Music : public GameObject
{
private:
    std::string m_music_file; ///< Path to the music/sound file.
    float m_volume;           ///< Volume level (0.0f to 1.0f).
    bool m_looping;           ///< Whether the music should loop indefinitely.
    bool isSoundEffect;       ///< Whether this Music object is a short sound effect.

public:
    /**
     * @brief Constructor for the Music class.
     */
    Music(GameState* gs, const std::string& name, const std::string& music_file,
        float volume = 1.0f, bool looping = true, bool isSoundEffect = false);

    /**
     * @brief Initializes the music or sound effect by loading the file.
     */
    void init() override;

    /**
     * @brief Updates the music state (currently does nothing).
     */
    void update(float dt) override;

    /**
     * @brief Draw method (no drawing for sounds/music).
     */
    void draw() override;

    /**
     * @brief Plays the music or sound effect.
     */
    void play();

    /**
     * @brief Stops the music (sound effects typically don't need this).
     */
    void stop();

    /**
     * @brief Sets the volume.
     * @param volume Volume level (0.0f to 1.0f).
     */
    void setVolume(float volume);

    /**
     * @brief Returns the current volume.
     */
    float getVolume() const { return m_volume; }

    /**
     * @brief Checks if the music is looping.
     */
    bool isLooping() const { return m_looping; }

    /**
     * @brief Checks if this is a sound effect.
     */
    bool getIsSoundEffect() const { return isSoundEffect; }
};
