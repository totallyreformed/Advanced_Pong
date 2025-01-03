#include "Obstacle.h"
#include "GameState.h"
#include "config.h"
#include "sgg/graphics.h"
#include <iostream>

Obstacle::Obstacle(GameState* gs, const std::string& name, Type type,
    float x, float y, float width, float height,
    int hit_points, float speed)
    : GameObject(gs, name),
    m_type(type),
    m_hit_points(hit_points),
    m_speed(speed)
{
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
}

void Obstacle::init()
{
    std::cout << "Obstacle '" << getName()
        << "' initialized at position: (" << getX() << ", " << getY() << ")\n";
}

void Obstacle::update(float dt)
{
    if (m_type == Type::Unbreakable)
    {
        // Move up/down within boundaries
        float new_y = getY() + m_speed * m_direction * dt;

        float canvasHeight = m_state->getCanvasHeight();
        float halfHeight = getHeight() / 2.0f;

        if (new_y + halfHeight >= canvasHeight)
        {
            new_y = canvasHeight - halfHeight;
            m_direction = -1;
        }
        else if (new_y - halfHeight <= 0.0f)
        {
            new_y = halfHeight;
            m_direction = 1;
        }
        setY(new_y);
    }
}

void Obstacle::draw()
{
    graphics::Brush br;

    if (m_type == Type::Breakable)
    {
        // Color depends on remaining hits
        if (m_hit_points >= 2) {
            // E.g. green when full
            br.fill_color[0] = 0.0f;
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }
        else {
            // E.g. more red if 1 hit remains
            br.fill_color[0] = 1.0f;
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.0f;
        }
    }
    else
    {
        // Unbreakable obstacle (e.g. gray)
        br.fill_color[0] = 0.5f;
        br.fill_color[1] = 0.5f;
        br.fill_color[2] = 0.5f;
    }

    br.outline_color[0] = 0.0f;
    br.outline_opacity = 0.0f;
    br.texture = "";

    graphics::drawRect(getX(), getY(), getWidth(), getHeight(), br);
}

void Obstacle::handleHit()
{
    if (m_type == Type::Breakable)
    {
        m_hit_points--;
        std::cout << "Obstacle '" << getName() << "' hit! HP: " << m_hit_points << "\n";
        if (m_hit_points <= 0)
        {
            setActive(false);
            std::cout << "Obstacle '" << getName() << "' destroyed!\n";
        }
    }
    // Unbreakable: no effect
}
