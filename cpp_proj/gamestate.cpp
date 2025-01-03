#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>
#include <chrono>
#include <thread>

// Initialize the static member to nullptr
GameState* GameState::m_unique_instance = nullptr;

/**
 * @brief Private constructor for GameState.
 */
GameState::GameState()
    : level(nullptr)
{
}

/**
 * @brief Destructor for GameState.
 */
GameState::~GameState()
{
}

/**
 * @brief Retrieves the unique instance of GameState.
 * @return Pointer to the unique GameState instance.
 */
GameState* GameState::getInstance()
{
    if (m_unique_instance == nullptr)
    {
        m_unique_instance = new GameState();
    }
    return m_unique_instance;
}

/**
 * @brief Releases the unique instance of GameState.
 */
void GameState::releaseInstance()
{
    if (m_unique_instance != nullptr)
    {
        delete m_unique_instance;
        m_unique_instance = nullptr;
    }
}

/**
 * @brief Retrieves the full asset path by appending the asset name to the asset directory.
 * @param asset The name of the asset file.
 * @return The complete path to the asset.
 */
std::string GameState::getFullAssetPath(const std::string& asset)
{
    return m_asset_path + asset;
}

/**
 * @brief Retrieves the asset directory path.
 * @return The asset directory path.
 */
std::string GameState::getAssetDir()
{
    return m_asset_path;
}

/**
 * @brief Initializes the game state, including levels and sounds.
 */
void GameState::init()
{
    // Initialize Level
    level = std::make_unique<Level>();
    level->init(1, true); // Start with Level 1

    std::cout << "GameState initialized. Level 1 is set.\n";
}

/**
 * @brief Updates all active game objects.
 * @param dt Delta time since the last update.
 */
void GameState::update(float dt)
{
    // Skip an update if a long delay is detected 
    // to avoid messing up the collision simulation
    if (dt > 500) // ms
        return;

    // Avoid too quick updates
    float sleep_time = std::max(17.0f - dt, 0.0f);
    if (sleep_time > 0.0f)
    {
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
    }

    if (!level)
        return;

    if (level)
    {
        level->update(dt);
    }
}

/**
 * @brief Draws all game objects and the level background.
 */
void GameState::draw() const
{
    if (level)
    {
        level->draw();
    }
}
