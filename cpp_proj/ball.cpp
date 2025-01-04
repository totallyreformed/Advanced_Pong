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

/**
 * @brief Constructs a new Ball object.
 *
 * Initializes the Ball with the specified parameters and resets its position and speed.
 *
 * @param gs Pointer to the current GameState.
 * @param name The name identifier for the Ball.
 * @param speed The base speed of the Ball.
 * @param width The width of the Ball.
 * @param height The height of the Ball.
 */
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

/**
 * @brief Initializes the Ball object.
 *
 * Resets the Ball's position, size, and powerups.
 */
void Ball::init()
{
    reset();
}

/**
 * @brief Updates the Ball's state.
 *
 * Handles the ramp-up speed after a reset, moves the Ball based on its current speed,
 * and updates any active powerups.
 *
 * @param dt Time elapsed since the last update in seconds.
 */
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

    // Move the Ball
    setX(getX() + m_speed_x * dt);
    setY(getY() + m_speed_y * dt);

    // Update active powerups
    updatePowerups();
}

/**
 * @brief Renders the Ball on the screen.
 *
 * Draws the Ball and visual indicators for ramp-up phases and active powerups.
 */
void Ball::draw()
{
    // Draw the main Ball
    graphics::Brush br;
    br.fill_color[0] = 1.0f; // White color
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;
    br.outline_opacity = 0.0f;
    br.texture = "";

    graphics::drawRect(getX(), getY(), getWidth(), getHeight(), br);

    // Draw ramp-up indicator if ramping up
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

/**
 * @brief Resets the Ball's position, size, and speed.
 *
 * Places the Ball at the center of the canvas, resets its size to the base dimensions,
 * clears any active powerups, and initializes its speed in a random direction.
 */
void Ball::reset()
{
    // Center the Ball on the canvas
    setX(m_state->getCanvasWidth() / 2.0f);
    setY(m_state->getCanvasHeight() / 2.0f);
    setWidth(m_base_width);
    setHeight(m_base_height);
    clearActivePowerups();

    m_reset_timer.stop();

    m_speed_x = 0.0f;
    m_speed_y = 0.0f;

    // Generate a random angle between 30-60 or 120-150 degrees
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist1(30.0f, 60.0f);
    std::uniform_real_distribution<float> dist2(120.0f, 150.0f);

    float angle = (gen() % 2 == 0) ? dist1(gen) : dist2(gen);
    float radians = angle * 3.14159265f / 180.0f;

    // Randomly decide to shoot left or right
    if ((gen() % 2) == 0)
    {
        radians = -radians;
    }

    // Calculate target speeds based on the angle
    m_target_speed_x = m_speed * std::cos(radians);
    m_target_speed_y = m_speed * std::sin(radians);

    // Start the ramp-up timer
    m_reset_timer.start();
    std::cout << "Ball reset with angle " << angle
        << " deg. speedX: " << m_target_speed_x
        << ", speedY: " << m_target_speed_y << "\n";
}

/**
 * @brief Applies a powerup effect to the Ball.
 *
 * Depending on the type of powerup, this method modifies the Ball's speed or size.
 * Certain powerups are ignored based on the current game state or existing active powerups.
 *
 * @param type The type of powerup to apply.
 */
void Ball::applyPowerup(Powerup::Type type)
{
    // Ignore SPEED_UP powerups in Sudden Death mode
    if (m_state->getCurrentLevel()->getLevelNumber() == 4 && type == Powerup::Type::SPEED_UP)
    {
        std::cout << "Ball is in Sudden Death. Ignoring SPEED_UP powerup.\n";
        return;
    }

    // Ignore powerups if ramp-up is in progress
    if (isRampingUp())
    {
        std::cout << "Ball is ramping up. Ignoring powerup.\n";
        return;
    }

    // Ignore powerups if a powerup is already active
    if (m_is_powerup_active)
    {
        std::cout << "Ball already has an active powerup. Ignoring new powerup.\n";
        return;
    }

    // Initialize the active powerup
    ActivePowerup ap;
    ap.type = type;
    ap.effect_timer = Timer(4.0f, Timer::TIMER_ONCE);
    ap.effect_timer.start();
    m_active_powerups.push_back(ap);

    // Mark that a powerup is now active
    m_is_powerup_active = true;

    // Apply the powerup effect based on its type
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

/**
 * @brief Updates the status of active powerups.
 *
 * Checks if any active powerup timers have expired and reverses their effects accordingly.
 */
void Ball::updatePowerups()
{
    for (auto it = m_active_powerups.begin(); it != m_active_powerups.end(); )
    {
        float progress = static_cast<float>(it->effect_timer);

        if (!it->effect_timer.isRunning())
        {
            // Reverse the powerup effect based on its type
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

            // Remove the expired powerup from the active list
            it = m_active_powerups.erase(it);

            // Reset the active powerup flag if no powerups remain
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

/**
 * @brief Clears all active powerups from the Ball.
 *
 * Resets the Ball's speed and size to their base values and removes any active powerup effects.
 */
void Ball::clearActivePowerups()
{
    // Clear the list of active powerups
    m_active_powerups.clear();

    // Reset the active powerup flag
    m_is_powerup_active = false;

    std::cout << "All active powerups have been cleared from the ball.\n";
}
