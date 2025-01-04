#pragma once
#include "GameObject.h"
#include "box.h"
#include <string>

/**
 * @class Obstacle
 * @brief Represents an obstacle in the Pong game.
 */
class Obstacle : public GameObject
{
public:
    enum class Type {
        Breakable,
        Unbreakable
    };

private:
	Type m_type;         ///< Type of obstacle.
    int m_hit_points;    ///< For breakable obstacles.
    float m_speed;       ///< For unbreakable obstacles.
    int m_direction = 1; ///< 1 for down, -1 for up.

public:
    /**
     * @brief Constructor for Obstacle.
     */
    Obstacle(GameState* gs, const std::string& name, Type type,
        float x, float y, float width, float height,
        int hit_points = 0, float speed = 0.0f);

    /**
     * @brief Initializes the obstacle.
     */
    void init() override;

    /**
     * @brief Updates the obstacle, moving it if unbreakable.
     */
    void update(float dt) override;

    /**
     * @brief Draws the obstacle on screen.
     */
    void draw() override;

    /**
     * @brief Handles a hit on a breakable obstacle.
     */
    void handleHit();

	/**
	* @brief Returns true if the obstacle is breakable.
	*/
    bool isBreakable() const { return m_type == Type::Breakable; }

	/**
	* @brief Returns the hit points of the obstacle.
	*/
    int getHitPoints() const { return m_hit_points; }

	/**
	* @brief Sets the hit points of the obstacle.
	*/
    void setHitPoints(int hp) { m_hit_points = hp; }

    /**
	* @brief Returns the width of the obstacle.
	*/
    float getWidth() const { return m_width; }

	/**
	* @brief Returns the height of the obstacle.
	*/
    float getHeight() const { return m_height; }

    /**
	* @brief Returns the speed of the obstacle.
	*/
    float getSpeed() const { return m_speed; }

	/**
	* @brief Returns the direction of the obstacle.
	*/
    int getDirection() const { return m_direction; }
};
