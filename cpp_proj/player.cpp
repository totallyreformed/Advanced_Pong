#include "Player.h"
#include "GameState.h"
#include <sgg/graphics.h>
#include "config.h"
#include <iostream>

/**
 * @brief Constructs a new Player object.
 *
 * Initializes the Player with the specified parameters, setting its position, movement keys,
 * paddle dimensions, and movement speed.
 *
 * @param gs Pointer to the current GameState.
 * @param name The name identifier for the Player.
 * @param posX The initial X-coordinate position of the Player.
 * @param posY The initial Y-coordinate position of the Player.
 * @param upKey The scancode for the key used to move the Player upwards.
 * @param downKey The scancode for the key used to move the Player downwards.
 * @param paddleWidth The width of the Player's paddle.
 * @param paddleHeight The height of the Player's paddle.
 */
Player::Player(GameState* gs, const std::string& name,
    float posX, float posY,
    graphics::scancode_t upKey, graphics::scancode_t downKey,
    float paddleWidth, float paddleHeight)
    : GameObject(gs, name),
    m_width(paddleWidth),
    m_height(paddleHeight),
    speed(1.0f),
    moveUpKey(upKey),
    moveDownKey(downKey)
{
    setX(posX);
    setY(posY);
}

/**
 * @brief Updates the Player's state based on user input and ensures it remains within boundaries.
 *
 * Handles the Player's vertical movement by checking if the assigned movement keys are pressed.
 * It moves the Player up or down accordingly and clamps the Player's position within the
 * predefined canvas boundaries to prevent it from moving off-screen.
 *
 * @param dt Delta time since the last update in seconds.
 */
void Player::update(float dt)
{
    // Move up if the up key is pressed
    if (graphics::getKeyState(moveUpKey))
    {
        y -= speed * dt;
    }

    // Move down if the down key is pressed
    if (graphics::getKeyState(moveDownKey))
    {
        y += speed * dt;
    }

    // Clamp the Player's Y-position within the canvas boundaries
    if (y + m_height / 2.0f > CANVAS_HEIGHT)
    {
        y = CANVAS_HEIGHT - m_height / 2.0f; // Prevent moving beyond the bottom boundary
    }
    if (y - m_height / 2.0f < 0.0f)
    {
        y = m_height / 2.0f; // Prevent moving beyond the top boundary
    }
}

/**
 * @brief Renders the Player's paddle on the screen.
 *
 * Draws the Player's paddle as a rectangle with a green fill and a white outline.
 * The paddle's dimensions and position are based on the Player's current state.
 */
void Player::draw()
{
    graphics::Brush br;

    // Set fill color to green for the paddle
    br.fill_color[0] = 0.0f; // Red component
    br.fill_color[1] = 1.0f; // Green component
    br.fill_color[2] = 0.0f; // Blue component

    // Set outline color to white for better visibility
    br.outline_color[0] = 1.0f; // Red component
    br.outline_color[1] = 1.0f; // Green component
    br.outline_color[2] = 1.0f; // Blue component
    br.outline_width = 2.0f;    // Thickness of the outline

    // Draw the paddle as a rectangle at the Player's current position and size
    graphics::drawRect(x, y, m_width, m_height, br);
}
