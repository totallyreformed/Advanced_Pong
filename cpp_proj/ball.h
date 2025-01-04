#pragma once
#include "GameObject.h"
#include <random>
#include <vector>
#include "timer.h"
#include "powerup.h"

/**
 * @class Ball
 * @brief Represents the ball in the Pong game.
 */
class Ball : public GameObject
{
private:
    Timer m_reset_timer;      // Timer for delay after scoring.
    float m_target_speed_x;   // Target speed on the x-axis after delay.
    float m_target_speed_y;   // Target speed on the y-axis after delay.
    float m_speed;            // Constant base speed of the ball.
    float m_speed_x;          // Current speed on the x-axis.
    float m_speed_y;          // Current speed on the y-axis.
	float m_base_width;	      // Base width of the ball.
	float m_base_height;	  // Base height of the ball.

    // Struct holding active powerup effects
    struct ActivePowerup {
        Powerup::Type type;
        Timer effect_timer;
    };
    std::vector<ActivePowerup> m_active_powerups;

    bool m_is_powerup_active = false;

public:
    Ball(GameState* gs, const std::string& name,
        float speed, float width, float height);

    void init() override;
    void update(float dt) override;
    void draw() override;

    /**
     * @brief Resets the ball to the center with a new random direction.
     */
    void reset();

    /**
     * @brief Applies a powerup effect to the ball immediately.
     */
    void applyPowerup(Powerup::Type type);

    /**
     * @brief Updates active powerup effects.
     */
    void updatePowerups();

    /**
     * @brief Clears all active powerups from the ball.
     */
    void clearActivePowerups();

    /**
     * @brief Getter for the ball's current speed on the x-axis.
     */
    float getSpeed_x() const { return m_speed_x; }

    /**
     * @brief Setter for the ball's speed on the x-axis.
     */
    void setSpeed_x(float speed_x) { m_speed_x = speed_x; }

    /**
     * @brief Getter for the ball's current speed on the y-axis.
     */
    float getSpeed_y() const { return m_speed_y; }

    /**
     * @brief Setter for the ball's speed on the y-axis.
     */
    void setSpeed_y(float speed_y) { m_speed_y = speed_y; }

    /**
     * @brief Getter for the ball's base speed.
     */
    float getSpeed() const { return m_speed; }

    /**
     * @brief Setter for the ball's base speed.
     */
    void setSpeed(float new_speed) { m_speed = new_speed; }

    /**
     * @brief Checks if the ball is currently ramping up its speed.
     * @return True if ramping up, False otherwise.
     */
    bool isRampingUp() { return m_reset_timer.isRunning(); }

    /**
     * @brief Checks if the ball has an active powerup
     * @return True if there is an active powerup, False otherwise.
     */
    bool isActivePowerup() const { return m_is_powerup_active; }
};
