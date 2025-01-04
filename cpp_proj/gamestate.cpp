#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>
#include <chrono>
#include <thread>

// Initialize the static member to nullptr
GameState* GameState::m_unique_instance = nullptr;

/**
 * @brief Private constructor for GameState.
 *
 * Initializes the GameState object. This constructor is private to enforce the singleton pattern,
 * ensuring that only one instance of GameState exists throughout the application's lifecycle.
 */
GameState::GameState()
    : level(nullptr)
{
    // Constructor body can be expanded if additional initialization is required
}

/**
 * @brief Destructor for GameState.
 *
 * Cleans up any resources held by GameState. Ensures that all dynamically allocated memory
 * and other resources are properly released when the GameState instance is destroyed.
 */
GameState::~GameState()
{
    // Destructor body can be expanded if additional cleanup is required
}

/**
 * @brief Retrieves the unique instance of GameState.
 *
 * Implements the singleton pattern by providing a global access point to the single GameState instance.
 * If the instance does not exist, it is created. Subsequent calls return the existing instance.
 *
 * @return Pointer to the unique GameState instance.
 */
GameState* GameState::getInstance()
{
    if (m_unique_instance == nullptr)
    {
        m_unique_instance = new GameState();
        std::cout << "GameState instance created.\n";
    }
    return m_unique_instance;
}

/**
 * @brief Releases the unique instance of GameState.
 *
 * Destroys the singleton instance and frees associated resources. After calling this method,
 * a new instance can be created by subsequent calls to getInstance().
 */
void GameState::releaseInstance()
{
    if (m_unique_instance != nullptr)
    {
        delete m_unique_instance;
        m_unique_instance = nullptr;
        std::cout << "GameState instance released.\n";
    }
}

/**
 * @brief Retrieves the full asset path by appending the asset name to the asset directory.
 *
 * Constructs the complete path to an asset by concatenating the base asset directory with the
 * provided asset filename. This is useful for loading resources such as images, sounds, and other assets.
 *
 * @param asset The name of the asset file (e.g., "background.png").
 * @return The complete path to the asset as a std::string.
 */
std::string GameState::getFullAssetPath(const std::string& asset)
{
    return m_asset_path + asset;
}

/**
 * @brief Retrieves the asset directory path.
 *
 * Provides access to the base directory where all game assets are stored. This directory is
 * used as the root path for loading various resources required by the game.
 *
 * @return The asset directory path as a std::string.
 */
std::string GameState::getAssetDir()
{
    return m_asset_path;
}

/**
 * @brief Initializes the game state, including levels and sounds.
 *
 * Sets up the initial game environment by creating and initializing the first level.
 * This method should be called once at the start of the game to prepare all necessary
 * components for gameplay.
 */
void GameState::init()
{
    // Initialize Level
    level = std::make_unique<Level>();
    level->init(1, true); // Start with Level 1 and display the main menu

    std::cout << "GameState initialized. Level 1 is set.\n";
}

/**
 * @brief Updates all active game objects.
 *
 * Handles the game logic by updating the current level and managing timing to ensure smooth
 * gameplay. This method is typically called once per frame with the elapsed time since the last update.
 *
 * @param dt Delta time since the last update in seconds.
 */
void GameState::update(float dt)
{
    // Skip an update if a long delay is detected to prevent simulation issues
    if (dt > 500) // ms
    {
        std::cout << "Skipped update due to excessive delta time: " << dt << " ms.\n";
        return;
    }

    // Ensure that updates do not occur too rapidly by enforcing a minimum frame time
    float sleep_time = std::max(17.0f - dt, 0.0f); // Approximately 60 FPS
    if (sleep_time > 0.0f)
    {
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
    }

    if (!level)
    {
        std::cout << "No active level to update.\n";
        return;
    }

    // Update the current level
    level->update(dt);
}

/**
 * @brief Draws all game objects and the level background.
 *
 * Delegates the rendering process to the current level, which handles drawing all active
 * game objects, such as players, the ball, obstacles, powerups, and background elements.
 */
void GameState::draw() const
{
    if (level)
    {
        level->draw();
    }
    else
    {
        std::cout << "No active level to draw.\n";
    }
}
