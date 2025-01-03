#include "Ball.h"
#include "GameState.h"
#include "sgg/graphics.h"
#include <cmath>
#include <iostream>
#include "config.h"
#include "box.h"
#include "music.h"
#include "powerup.h"
#include "clamp.h"

// Constructor
Ball::Ball(GameState* gs, const std::string& name,
    float speed, float width, float height)
    : GameObject(gs, name),
    m_speed(speed),
    m_speed_x(speed),
    m_speed_y(speed),
    m_reset_timer(1.0f, Timer::TIMER_ONCE),
	m_base_width(width),
	m_base_height(height)
{
    setWidth(width);
    setHeight(height);
    reset();
    std::cout << "Ball created at (" << getX() << ", " << getY() << ")\n";
}

void Ball::init()
{
    reset();
}

void Ball::update(float dt)
{
    // Handle reset timer for ramp-up speed
    if (m_reset_timer.isRunning())
    {
        float progress = static_cast<float>(m_reset_timer);
        // Ramp up speeds
        m_speed_x = m_target_speed_x * progress;
        m_speed_y = m_target_speed_y * progress;

        if (progress >= 1.0f)
        {
            m_reset_timer.stop();
            m_speed_x = m_target_speed_x;
            m_speed_y = m_target_speed_y;
            std::cout << "Speed ramp-up complete. speed_x: " << m_speed_x
                << ", speed_y: " << m_speed_y << "\n";
        }
    }

    // Move the ball
    setX(getX() + m_speed_x * dt);
    setY(getY() + m_speed_y * dt);

    // Finally, apply any active powerup timeouts
    updatePowerups();
}

void Ball::draw()
{
    graphics::Brush br;
    br.fill_color[0] = 1.0f; // White color
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;
    br.outline_opacity = 0.0f;
    br.texture = "";

    graphics::drawRect(getX(), getY(), getWidth(), getHeight(), br);

    // **Draw ramp-up indicator if ramping up**
    if (isRampingUp())
    {
        graphics::Brush ramp_br;
        ramp_br.fill_color[0] = 1.0f; // Red color
        ramp_br.fill_color[1] = 0.0f;
        ramp_br.fill_color[2] = 0.0f;
        ramp_br.fill_opacity = 0.5f;
        ramp_br.outline_opacity = 0.0f;

        graphics::drawDisk(getX(), getY(), getWidth(), ramp_br);
    }

    // Draw active powerup indicator
	if (isActivePowerup())
	{
		graphics::Brush powerup_br;
		powerup_br.fill_color[0] = 1.0f; // Yellow color
		powerup_br.fill_color[1] = 1.0f;
		powerup_br.fill_color[2] = 0.0f;
		powerup_br.fill_opacity = 1.0f;
		powerup_br.outline_opacity = 1.0f;

		graphics::drawRect(getX(), getY(), getWidth(), getHeight(), powerup_br);
	}
}

void Ball::reset()
{
    setX(m_state->getCanvasWidth() / 2.0f);
    setY(m_state->getCanvasHeight() / 2.0f);
    setWidth(m_base_width);
    setHeight(m_base_height);
    clearActivePowerups();

    m_reset_timer.stop();

    m_speed_x = 0.0f;
    m_speed_y = 0.0f;

    // Random angle between 30-60 or 120-150 deg
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist1(30.0f, 60.0f);
    std::uniform_real_distribution<float> dist2(120.0f, 150.0f);

    float angle = (gen() % 2 == 0) ? dist1(gen) : dist2(gen);
    float radians = angle * 3.14159265f / 180.0f;

    // Random direction (left or right)
    if ((gen() % 2) == 0)
    {
        radians = -radians;
    }

    m_target_speed_x = m_speed * std::cos(radians);
    m_target_speed_y = m_speed * std::sin(radians);

    m_reset_timer.start();
    std::cout << "Ball reset with angle " << angle
        << " deg. speedX: " << m_target_speed_x
        << ", speedY: " << m_target_speed_y << "\n";
}

void Ball::applyPowerup(Powerup::Type type)
{
    // **Ignore SPEED_UP powerups in Sudden Death**
    if (m_state->getCurrentLevel()->getLevelNumber() == 4 && type == Powerup::Type::SPEED_UP)
    {
        std::cout << "Ball is in Sudden Death. Ignoring SPEED_UP powerup.\n";
        return;
    }

    // **Ignore powerups if ramp-up is in progress**
    if (isRampingUp())
    {
        std::cout << "Ball is ramping up. Ignoring powerup.\n";
        return;
    }

    // **Ignore powerups if a powerup is already active**
    if (m_is_powerup_active)
    {
        std::cout << "Ball already has an active powerup. Ignoring new powerup.\n";
        return;
    }

    ActivePowerup ap;
    ap.type = type;
    ap.effect_timer = Timer(4.0f, Timer::TIMER_ONCE);
    ap.effect_timer.start();
    m_active_powerups.push_back(ap);

    // Mark that a powerup is now active
	m_is_powerup_active = true;

    switch (type)
    {
    case Powerup::Type::SPEED_UP:
        m_speed_x *= 1.5f;
        m_speed_y *= 1.5f;
        std::cout << "POWERUP: SPEED_UP applied.\n";
        break;
    case Powerup::Type::SLOW_DOWN:
        m_speed_x *= 0.60f;
        m_speed_y *= 0.60f;
        std::cout << "POWERUP: SLOW_DOWN applied.\n";
        break;
    case Powerup::Type::INCREASE_SIZE:
        setWidth(m_base_width * 1.4f);
        setHeight(m_base_height * 1.4f);
        std::cout << "POWERUP: INCREASE_SIZE applied.\n";
        break;
    case Powerup::Type::DECREASE_SIZE:
        setWidth(m_base_width * 0.7f);
        setHeight(m_base_height * 0.7f);
        std::cout << "POWERUP: DECREASE_SIZE applied.\n";
        break;
    }
}

void Ball::updatePowerups()
{
    for (auto it = m_active_powerups.begin(); it != m_active_powerups.end(); )
    {
        float progress = static_cast<float>(it->effect_timer);

        if (!it->effect_timer.isRunning())
        {
            // Reverse powerup effect
            switch (it->type)
            {
            case Powerup::Type::SPEED_UP:
                m_speed_x /= 1.5f;
                m_speed_y /= 1.5f;
                std::cout << "POWERUP: SPEED_UP expired.\n";
                break;
            case Powerup::Type::SLOW_DOWN:
                m_speed_x /= 0.60f;
                m_speed_y /= 0.60f;
                std::cout << "POWERUP: SLOW_DOWN expired.\n";
                break;
            case Powerup::Type::INCREASE_SIZE:
                setWidth(m_base_width);
                setHeight(m_base_height);
                std::cout << "POWERUP: INCREASE_SIZE expired.\n";
                break;
            case Powerup::Type::DECREASE_SIZE:
                setWidth(m_base_width);
                setHeight(m_base_height);
                std::cout << "POWERUP: DECREASE_SIZE expired.\n";
                break;
            }
            it = m_active_powerups.erase(it);

            // Reset active powerup flag
            if (m_active_powerups.empty())
            {
                m_is_powerup_active = false;
                std::cout << "No active powerups remaining.\n";
            }
        }
        else
        {
            ++it;
        }
    }
}

void Ball::clearActivePowerups()
{
    // Clear the list of active powerups
	m_active_powerups.clear();

    m_is_powerup_active = false;

    std::cout << "All active powerups have been cleared from the ball.\n";
}