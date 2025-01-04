#pragma once

#include <memory>
#include "Level.h"
#include "config.h"
#include "menu.h"

/**
 * @class GameState
 * @brief Manages the overall state of the game, including levels and global settings.
 * Implements the Singleton pattern to ensure a unique instance.
 */
class GameState
{
private:
    // Singleton instance
    static GameState* m_unique_instance;

    // Asset directory path
    std::string m_asset_path = "assets\\";

    // Unique pointer to the current Level and menu
    std::unique_ptr<Level> level;

    /**
     * @brief Private constructor to prevent external instantiation.
     */
    GameState();

    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    GameState(const GameState&) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent assignment.
     */
    GameState& operator=(const GameState&) = delete;

public:
    bool music_on = true; ///< Flag to control music playback.
    bool m_debugging = false;

    /**
     * @brief Destructor.
     */
    ~GameState();

    /**
     * @brief Retrieves the unique instance of GameState.
     * @return Pointer to the unique GameState instance.
     */
    static GameState* getInstance();

    /**
     * @brief Releases the unique instance of GameState.
     */
    void releaseInstance();

    /**
     * @brief Retrieves the full asset path by appending the asset name to the asset directory.
     * @param asset The name of the asset file.
     * @return The complete path to the asset.
     */
    std::string getFullAssetPath(const std::string& asset);

    /**
     * @brief Retrieves the asset directory path.
     * @return The asset directory path.
     */
    std::string getAssetDir();

    // Get current level
	Level* getCurrentLevel() const { return level.get(); }

    /**
     * @brief Initializes the game state, including levels and sounds.
     */
    void init();

    /**
     * @brief Updates all active game objects.
     * @param dt Delta time since the last update.
     */
    void update(float dt);

    /**
     * @brief Draws all game objects and the level background.
     */
    void draw() const;

	/**
	* @brief Retrieves the width of the game canvas.
	* @return The width of the canvas.
	*/
	float getCanvasWidth() const { return CANVAS_WIDTH; }

	/**
	* @brief Retrieves the height of the game canvas.
	* @return The height of the canvas.
	*/
	float getCanvasHeight() const { return CANVAS_HEIGHT; }
};
