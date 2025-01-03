#pragma once

#include <vector>
#include <memory>
#include "Player.h"
#include "Ball.h"
#include "Obstacle.h"
#include "Powerup.h"
#include "Music.h"
#include "Menu.h"
#include "GameObject.h"
#include "config.h"
#include "sgg/graphics.h"

/**
 * @class Level
 * @brief Manages all objects and logic for a single level, including players, the ball,
 *        obstacles, powerups, and background drawing. Also handles level progression.
 */
class Level
{
private:
    // Current level number (1-4)
    int m_level_number = 1;

    // Timer for each level (1 minute = 60.0f seconds)
    float m_level_timer = 300.0f;

    // Powerup spawning variables
    float m_elapsed_time;                     // Tracks elapsed time since level start
    float m_next_powerup_spawn_time;          // Time at which the next powerup should spawn
    int m_total_powerups_to_spawn;            // Total number of powerups to spawn in the level
    int m_powerups_spawned;                   // Number of powerups that have been spawned so far
	int m_next_obstacle_spawn_time = 0.0f; 		 // Time at which the next obstacle should spawn
    int m_next_breakable_obstacle_spawn_time_level4; // Time at which the next obstacle should spawn in Level 4
	float m_speed_multiplier; 			 // Speed multiplier to track active powerups affecting the ball's speed

    std::vector<std::pair<float, float>> m_powerup_spawn_positions; // Predefined spawn positions for powerups

    // **Predefined Spawn Positions for Level 4 Obstacles**
    // Ensure these positions are spaced adequately
    std::vector<std::pair<float, float>> m_unbreakable_obstacle_spawn_positions_level4 = {
        {200.0f, 200.0f},  // First unbreakable obstacle position
        {600.0f, 600.0f}   // Second unbreakable obstacle position
    };

    std::vector<std::pair<float, float>> m_breakable_obstacle_spawn_positions_level4 = {
        {200.0f, 600.0f},  // First breakable obstacle position
        {600.0f, 200.0f}   // Second breakable obstacle position
    };

    // **Predefined Spawn Positions for Level 4 Powerups**
    // Ensure these positions are spaced adequately from each other and from obstacles
    std::vector<std::pair<float, float>> m_powerup_spawn_positions_level4 = {
        {400.0f, 300.0f},
        {400.0f, 500.0f},
        {300.0f, 400.0f},
        {500.0f, 400.0f}
    };

    // Maximum counts
    static const int MAX_UNBREAKABLE_OBSTACLES = 2;
	static const int MAX_BREAKABLE_OBSTACLES = 2;
    static const int MAX_POWERUPS = 4;

    // Counters to track spawned obstacles and powerups in Level 4
    // **Counters to Track Spawned Obstacles in Level 4**
    int m_obstacles_spawned_level4 = 0;
    int m_unbreakable_obstacles_spawned_level4 = 0;
    int m_breakable_obstacles_spawned_level4 = 0;
    int m_powerups_spawned_level4 = 0;

    // Next spawn times for obstacles and powerups in Level 4
    float m_next_obstacle_spawn_time_level4 = 0.0f;
    float m_next_powerup_spawn_time_level4 = 0.0f;

    // Each player has its own score to determine who wins in the end
    int m_player1_score = 0;
    int m_player2_score = 0;

	// Winner of the match (0 = no winner yet, 1 = player 1, 2 = player 2)
    int m_winner = 0;

    // Players
    std::unique_ptr<Player> m_player1;
    std::unique_ptr<Player> m_player2;

    // Ball
    std::unique_ptr<Ball> m_ball;

    // Obstacles and Powerups
    std::vector<std::unique_ptr<Obstacle>> m_obstacles;
    std::vector<std::unique_ptr<Powerup>> m_powerups;

    // Background Brush
    graphics::Brush m_bg_brush;

    // Music and Sound Effects
	std::unique_ptr<Music> m_background_music;
	std::unique_ptr<Music> m_paddle_hit_sound;
	std::unique_ptr<Music> m_powerup_sound;

    // Menu
	std::unique_ptr<Menu> m_menu;

    /**
     * @enum LevelState
     * @brief Defines the current state within a level.
     */
    enum class LevelState
    {
        MAIN_MENU,    ///< Main Menu before starting the level.
        ACTIVE,       ///< Active gameplay.
        PAUSE_MENU,   ///< Pause Menu between levels.
        GAME_OVER     ///< Game Over screen.
    };

    LevelState m_level_state = LevelState::MAIN_MENU; ///< Current state within the level.


    /**
     * @brief Sets up the game objects specific to a given level (1-4).
     * @param level_number The level number to set up.
     */
    void setupLevelObjects(int level_number);

    /**
     * @brief Checks if it's time to advance to the next level based on timer or player lives.
     */
    void checkLevelProgression();

    /**
     * @brief Moves to the next level (or to "sudden death" if level 3 is over).
     */
    void nextLevel();

public:
    /**
     * @brief Initializes the level: sets up players, ball, obstacles, powerups
     *        depending on the current level number (1-4).
     * @param level_number The level number to initialize.
     */
    void init(int level_number = 1, bool show_menu = true);

    /**
     * @brief Updates all objects in the level, checks for collisions, and
     *        handles timer / lives logic to proceed to next level.
     * @param dt Time elapsed since the last update in seconds.
     */
    void update(float dt);

    /**
     * @brief Draws all level objects (players, ball, obstacles, powerups) and background.
     */
    void draw() const;

    /**
     * @brief Retrieves the current level number.
     * @return The current level number as an integer.
     */
    int getLevelNumber() const { return m_level_number; }

    /**
     * @brief Destructor for the Level class.
     */
    ~Level();
};
