#pragma once
#include "GameObject.h"
#include "sgg/scancodes.h"
#include <string>

/**
 * @class Player
 * @brief Represents a player-controlled paddle in the game.
 */
class Player : public GameObject
{
private:
    float m_width;
    float m_height;
    float speed;

    graphics::scancode_t moveUpKey;
    graphics::scancode_t moveDownKey;

public:
    /**
     * @brief Constructor for the Player class.
     */
    Player(GameState* gs, const std::string& name,
        float posX, float posY,
        graphics::scancode_t upKey, graphics::scancode_t downKey,
        float paddleWidth, float paddleHeight);

    /**
     * @brief Updates the player's position and input.
     */
    void update(float dt) override;

    /**
     * @brief Draws the player paddle on screen.
     */
    void draw() override;

    // Getters
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
	float getSpeed() const { return speed; }
};
