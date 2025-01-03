#include "Player.h"
#include "GameState.h"
#include <sgg/graphics.h>
#include "config.h"
#include <iostream>

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

void Player::update(float dt)
{
    // Move up
    if (graphics::getKeyState(moveUpKey))
    {
        y -= speed * dt;
    }

    // Move down
    if (graphics::getKeyState(moveDownKey))
    {
        y += speed * dt;
    }

    // Clamp position within canvas boundaries
    if (y + m_height / 2.0f > CANVAS_HEIGHT)
    {
        y = CANVAS_HEIGHT - m_height / 2.0f;
    }
    if (y - m_height / 2.0f < 0.0f)
    {
        y = m_height / 2.0f;
    }
}

void Player::draw()
{
    graphics::Brush br;
    br.fill_color[0] = 0.0f; // Green color
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 0.0f;

    br.outline_color[0] = 1.0f; // White outline
    br.outline_color[1] = 1.0f;
    br.outline_color[2] = 1.0f;
    br.outline_width = 2.0f;

    graphics::drawRect(x, y, m_width, m_height, br);
}
