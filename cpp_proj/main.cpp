// main.cpp
#include "GameState.h"
#include <sgg/graphics.h>
#include <memory>
#include <string>
#include "config.h"
#include <iostream>

/**
 * @brief Draw callback function.
 * Calls the GameState's draw method.
 */
void draw()
{
    if (GameState::getInstance()) // Access via Singleton
    {
        GameState::getInstance()->draw();
    }
}

/**
 * @brief Update callback function.
 * Calls the GameState's update method.
 * @param dt Delta time since the last update.
 */
void update(float dt)
{
    if (GameState::getInstance())
    {
        GameState::getInstance()->update(dt);
    }
}

int main()
{
    // Initialize game window with canvas size 900x900
    graphics::createWindow(900, 900, "Advanced Pong");

    // Set canvas scale mode
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_WINDOW);

    // Set callback functions
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // Initialize GameState (Singleton instance is managed internally)
    GameState::getInstance()->init();

    // Start the game loop
    graphics::startMessageLoop();

    // Cleanup (optional, depending on implementation)
    // Currently, the Singleton instance is not deleted automatically
    GameState::getInstance()->releaseInstance();

    graphics::destroyWindow();
    return 0;
}
