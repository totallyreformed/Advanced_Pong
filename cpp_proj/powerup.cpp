#include "powerup.h"
#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>

/**
 * @brief Constructs a new Powerup object.
 *
 * Initializes the Powerup with the specified parameters, setting its type, position,
 * and default dimensions. Determines whether the Powerup has an associated texture.
 *
 * @param gs Pointer to the current GameState.
 * @param name The name identifier for the Powerup.
 * @param type The type of Powerup (e.g., SPEED_UP, SLOW_DOWN).
 * @param x The initial X-coordinate position of the Powerup.
 * @param y The initial Y-coordinate position of the Powerup.
 */
Powerup::Powerup(GameState* gs, const std::string& name, Type type, float x, float y)
    : GameObject(gs, name),
    m_type(type),
    m_texture_file("")
{
    setX(x);
    setY(y);
    setWidth(50.0f);  // Default width for all Powerups
    setHeight(50.0f); // Default height for all Powerups
}

/**
 * @brief Initializes the Powerup object.
 *
 * Assigns the appropriate texture file based on the Powerup type and logs its initialization.
 */
void Powerup::init()
{
    // Assign texture based on Powerup type
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
        m_texture_file = ""; // No texture for undefined types
        break;
    }

    // Log the initialization details of the Powerup
    std::cout << "Powerup '" << getName()
        << "' of type " << static_cast<int>(m_type)
        << " initialized at (" << getX() << ", " << getY() << ")\n";
}

/**
 * @brief Updates the Powerup's state.
 *
 * Currently, Powerups are static and do not require movement or state changes.
 * This method is intentionally left empty but can be expanded for dynamic Powerups.
 *
 * @param dt Delta time since the last update in seconds.
 */
void Powerup::update(float dt)
{
    // Static Powerup; no movement or state changes required.
    (void)dt; // Prevent unused parameter warning
}

/**
 * @brief Renders the Powerup on the screen.
 *
 * Draws the Powerup as a rectangle with its assigned texture. If no texture is assigned,
 * it defaults to a white color. The Powerup's dimensions and position are based on its current state.
 */
void Powerup::draw()
{
    graphics::Brush br;

    // Assign texture if available; otherwise, use a default fill color
    if (!m_texture_file.empty())
    {
        br.texture = m_state->getFullAssetPath(m_texture_file);
        br.fill_color[0] = 1.0f; // Red component (unused if texture is present)
        br.fill_color[1] = 1.0f; // Green component (unused if texture is present)
        br.fill_color[2] = 1.0f; // Blue component (unused if texture is present)
    }
    else
    {
        br.fill_color[0] = 1.0f; // White color if no texture is present
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;
    }

    br.outline_color[0] = 0.0f;    // No outline color
    br.outline_opacity = 0.0f;     // No outline opacity

    // Draw the Powerup as a rectangle at its current position and size
    graphics::drawRect(getX(), getY(), getWidth(), getHeight(), br);
}
