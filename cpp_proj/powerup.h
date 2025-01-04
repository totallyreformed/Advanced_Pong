#pragma once
#include "GameObject.h"
#include <string>

/**
 * @class Powerup
 * @brief Represents a powerup that affects the ball's behavior for a limited time.
 */
class Powerup : public GameObject
{
public:
    enum class Type {
        SPEED_UP,
        SLOW_DOWN,
        INCREASE_SIZE,
        DECREASE_SIZE
    };

private:
    Type m_type;                    // Type of the powerup
    std::string m_texture_file;     // Asset for the powerup

public:
    Powerup(GameState* gs, const std::string& name, Type type, float x, float y);

    void init() override;
    void update(float dt) override;
    void draw() override;

    Type getType() const { return m_type; }
};
