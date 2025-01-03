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
    Type m_type;
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

    bool isBreakable() const { return m_type == Type::Breakable; }

    int getHitPoints() const { return m_hit_points; }
    void setHitPoints(int hp) { m_hit_points = hp; }

    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }

    float getSpeed() const { return m_speed; }
    int getDirection() const { return m_direction; }
};
