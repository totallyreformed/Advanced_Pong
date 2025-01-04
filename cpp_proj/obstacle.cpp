#include "Obstacle.h"
#include "GameState.h"
#include "config.h"
#include "sgg/graphics.h"
#include <iostream>

/**
 * @brief Constructs a new Obstacle object.
 *
 * Initializes the Obstacle with the specified parameters, setting its type, hit points,
 * speed, and position within the game world.
 *
 * @param gs Pointer to the current GameState.
 * @param name The name identifier for the Obstacle.
 * @param type The type of obstacle (Breakable or Unbreakable).
 * @param x The initial X-coordinate position of the Obstacle.
 * @param y The initial Y-coordinate position of the Obstacle.
 * @param width The width of the Obstacle.
 * @param height The height of the Obstacle.
 * @param hit_points The number of hits the Obstacle can take before being destroyed (only for Breakable obstacles).
 * @param speed The movement speed of the Obstacle (only for Unbreakable obstacles).
 */
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

/**
 * @brief Initializes the Obstacle object.
 *
 * Logs the initialization of the Obstacle, including its name and position.
 */
void Obstacle::init()
{
    std::cout << "Obstacle '" << getName()
        << "' initialized at position: (" << getX() << ", " << getY() << ")\n";
}

/**
 * @brief Updates the Obstacle's state.
 *
 * Handles the movement logic for Unbreakable obstacles, allowing them to move
 * up and down within the game boundaries. Breakable obstacles remain stationary.
 *
 * @param dt Delta time since the last update in seconds.
 */
void Obstacle::update(float dt)
{
    if (m_type == Type::Unbreakable)
    {
        // Calculate the new Y-position based on speed and direction
        float new_y = getY() + m_speed * m_direction * dt;

        float canvasHeight = m_state->getCanvasHeight();
        float halfHeight = getHeight() / 2.0f;

        // Check for collision with the top boundary
        if (new_y + halfHeight >= canvasHeight)
        {
            new_y = canvasHeight - halfHeight; // Prevent moving out of bounds
            m_direction = -1; // Reverse direction to move downward
        }
        // Check for collision with the bottom boundary
        else if (new_y - halfHeight <= 0.0f)
        {
            new_y = halfHeight; // Prevent moving out of bounds
            m_direction = 1; // Reverse direction to move upward
        }

        setY(new_y); // Update the Obstacle's Y-position
    }
}

/**
 * @brief Renders the Obstacle on the screen.
 *
 * Draws the Obstacle with colors indicating its type and current state.
 * Breakable obstacles change color based on remaining hit points, while
 * Unbreakable obstacles are rendered in a consistent gray color.
 */
void Obstacle::draw()
{
    graphics::Brush br;

    if (m_type == Type::Breakable)
    {
        // Set color based on remaining hit points
        if (m_hit_points >= 2) {
            // Green color when hit points are 2 or more
            br.fill_color[0] = 0.0f;
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 0.0f;
        }
        else {
            // Red color when hit points are less than 2
            br.fill_color[0] = 1.0f;
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.0f;
        }
    }
    else
    {
        // Gray color for Unbreakable obstacles
        br.fill_color[0] = 0.5f;
        br.fill_color[1] = 0.5f;
        br.fill_color[2] = 0.5f;
    }

    br.outline_color[0] = 0.0f;
    br.outline_opacity = 0.0f; // No outline
    br.texture = ""; // No texture

    // Draw the Obstacle as a rectangle at its current position and size
    graphics::drawRect(getX(), getY(), getWidth(), getHeight(), br);
}

/**
 * @brief Handles a hit on the Obstacle.
 *
 * Decrements the hit points of a Breakable obstacle and checks if it should be destroyed.
 * Unbreakable obstacles are unaffected by hits.
 */
void Obstacle::handleHit()
{
    if (m_type == Type::Breakable)
    {
        m_hit_points--; // Decrement hit points
        std::cout << "Obstacle '" << getName() << "' hit! HP: " << m_hit_points << "\n";

        if (m_hit_points <= 0)
        {
            setActive(false); // Deactivate the Obstacle if hit points are depleted
            std::cout << "Obstacle '" << getName() << "' destroyed!\n";
        }
    }
    // Unbreakable obstacles do not respond to hits
}
