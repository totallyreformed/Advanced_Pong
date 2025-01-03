#include "powerup.h"
#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>

Powerup::Powerup(GameState* gs, const std::string& name, Type type, float x, float y)
    : GameObject(gs, name),
    m_type(type),
    m_texture_file("")
{
    setX(x);
    setY(y);
    setWidth(50.0f);
    setHeight(50.0f);
}

void Powerup::init()
{
    // Assign texture based on powerup type
    switch (m_type)
    {
    case Type::SPEED_UP:
        m_texture_file = "speed_up.png";
        break;
    case Type::SLOW_DOWN:
        m_texture_file = "slow_down.png";
        break;
    case Type::INCREASE_SIZE:
        m_texture_file = "increase_size.png";
        break;
    case Type::DECREASE_SIZE:
        m_texture_file = "decrease_size.png";
        break;
    default:
        m_texture_file = "";
        break;
    }
    std::cout << "Powerup '" << getName()
        << "' of type " << static_cast<int>(m_type)
        << " initialized at (" << getX() << ", " << getY() << ")\n";
}

void Powerup::update(float dt)
{
    // Static powerup; no movement by default.
    (void)dt;
}

void Powerup::draw()
{
    graphics::Brush br;
    br.texture = m_state->getFullAssetPath(m_texture_file);
    br.fill_color[0] = 1.0f; // White color if texture is present
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;
    br.outline_color[0] = 0.0f;
    br.outline_opacity = 0.0f;

    graphics::drawRect(getX(), getY(), getWidth(), getHeight(), br);
}
