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
	float m_width;         // Width of the player paddle.
	float m_height;		   // Height of the player paddle.
	float speed;		   // Speed of the player paddle.

	graphics::scancode_t moveUpKey;      // Key to move the paddle up.
	graphics::scancode_t moveDownKey;    // Key to move the paddle down.

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
