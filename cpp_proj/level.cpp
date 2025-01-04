#include "Level.h"
#include "GameState.h"
#include "sgg/graphics.h"
#include <iostream>
#include <cmath>
#include <random>

/**
 * @brief Destructor for the Level class.
 *
 * Responsible for cleaning up resources used by the Level. Since unique pointers are utilized
 * for managing game objects, memory deallocation is handled automatically.
 */
Level::~Level()
{
    // Unique pointers automatically handle memory deallocation.
}

/**
 * @brief Initializes the level by setting up all necessary game objects and configurations.
 *
 * This method configures the level based on the provided level number. It initializes spawning variables,
 * sets up background music and sound effects, and prepares the game menu if required.
 *
 * @param level_number The level number to initialize (1-4).
 * @param show_menu A boolean flag indicating whether to display the menu upon initialization.
 */
void Level::init(int level_number, bool show_menu)
{
    // Set the current level number
    m_level_number = level_number;
    //m_level_number = 4;
    // Debug: Sudden death mode initialization

    // Initialize powerup spawning variables
    m_elapsed_time = 0.0f;
    m_next_powerup_spawn_time = 5.0f; // Start spawning after 5 seconds
    m_powerups_spawned = 0;

    // Initialize obstacle spawning variables (Sudden Death)
	m_next_obstacle_spawn_time = 7.5f;

    // **Initialize speed multiplier**
    m_speed_multiplier = 1.0f;

    // Define powerup spawn positions based on level
    if (level_number == 2)
    {
        m_powerup_spawn_positions = { {300.0f, 300.0f}, {600.0f, 300.0f}, {400.0f, 500.0f}, {500.0f, 200.0f} };
        m_total_powerups_to_spawn = 4; // Total powerups for Level 2
    }
    else if (level_number == 3)
    {
        m_powerup_spawn_positions = { {500.0f, 500.0f}, {700.0f, 200.0f}, {200.0f, 600.0f}, {400.0f, 400.0f} };
        m_total_powerups_to_spawn = 4; // Total powerups for Level 3
    }
    else
    {
        // Reset spawn counters (Spawned Obstacles counter for Level 4)
        m_unbreakable_obstacles_spawned_level4 = 0;
        m_breakable_obstacles_spawned_level4 = 0;
        m_powerups_spawned_level4 = 0;
    }

    // Reset the level timer to 30 seconds
    m_level_timer = 30.0f;

    // Clear existing obstacles and powerups
    m_obstacles.clear();
    m_powerups.clear();

    // Setup game objects specific to the current level
    setupLevelObjects(m_level_number);

    // Setup the background brush
    m_bg_brush.texture = GameState::getInstance()->getFullAssetPath("background.png");
    m_bg_brush.fill_opacity = 0.17f;
    m_bg_brush.outline_opacity = 0.0f;

    // Initialize and Play Background Music
    m_background_music = std::make_unique<Music>(GameState::getInstance(), "BackgroundMusic", "background_music.mp3", 1.0f, true, false);
    if (m_background_music)
    {
        m_background_music->play();
    }

    // Initialize Sound Effects
    m_paddle_hit_sound = std::make_unique<Music>(GameState::getInstance(), "PaddleHit", "paddle_hit.wav", 0.6f, false, true);
    m_powerup_sound = std::make_unique<Music>(GameState::getInstance(), "PowerupSound", "paddle_hit.wav", 0.6f, false, true);

    if (show_menu)
    {
        // **Determine Menu Type Based on Level Number**
        MenuType current_menu_type = (level_number == 1) ? MenuType::MAIN_MENU : MenuType::PAUSE_MENU;

        // Initialize Menu with the determined type
        m_menu = std::make_unique<Menu>(current_menu_type);
        m_level_state = (current_menu_type == MenuType::MAIN_MENU) ? LevelState::MAIN_MENU : LevelState::PAUSE_MENU;

        std::cout << "Level " << m_level_number << " initialized with "
            << ((current_menu_type == MenuType::MAIN_MENU) ? "Main Menu." : "Pause Menu.") << "\n";
    }
    else
    {
        m_level_state = LevelState::ACTIVE;
        std::cout << "Level " << m_level_number << " initialized and active.\n";
    }

    std::cout << "Level " << m_level_number << " initialized.\n";
}

/**
 * @brief Sets up the game objects specific to a given level (1-4).
 *
 * This method initializes players, the ball, obstacles, and powerups based on the current level number.
 *
 * @param level_number The level number to set up.
 */
void Level::setupLevelObjects(int level_number)
{
    // Initialize Players iwth assigned movement keys and paddle dimensions
    m_player1 = std::make_unique<Player>(
        GameState::getInstance(), "Player1", 50.0f, CANVAS_HEIGHT / 2.0f,
        graphics::SCANCODE_W, graphics::SCANCODE_S, 10.0f, 70.0f
    );
    m_player1->init();

    m_player2 = std::make_unique<Player>(
        GameState::getInstance(), "Player2", CANVAS_WIDTH - 50.0f, CANVAS_HEIGHT / 2.0f,
        graphics::SCANCODE_UP, graphics::SCANCODE_DOWN, 10.0f, 70.0f
    );
    m_player2->init();

    // Initialize Ball with specified speed and dimensions
    m_ball = std::make_unique<Ball>(
        GameState::getInstance(), "Ball", 0.7f, 15.0f, 15.0f
    );
    m_ball->init();

    // Initialize Obstacles and Powerups based on the level number
    if (level_number == 1)
    {
        // Level 1: Classic Pong (no obstacles or powerups)
        std::cout << "Level 1: Classic Pong. No obstacles or powerups.\n";
    }
    else if (level_number == 2)
    {
        // Level 2: Add 2 stationary breakable obstacles + 2 powerups
        std::cout << "Level 2: Adding 2 breakable obstacles and 2 powerups.\n";

        // Add Breakable Obstacles
        auto obstacle1 = std::make_unique<Obstacle>(
            GameState::getInstance(), "BreakableObstacle1", Obstacle::Type::Breakable,
            450.0f, 700.0f, 10.0f, 100.0f, 2, 0.0f
        );
        obstacle1->init();
        m_obstacles.push_back(std::move(obstacle1));

        auto obstacle2 = std::make_unique<Obstacle>(
            GameState::getInstance(), "BreakableObstacle2", Obstacle::Type::Breakable,
            450.0f, 250.0f, 10.0f, 100.0f, 2, 0.0f
        );
        obstacle2->init();
        m_obstacles.push_back(std::move(obstacle2));
    }
    else if (level_number == 3)
    {
        // Level 3: Add 2 stationary breakable obstacles, 1 moving unbreakable obstacle + 3 powerups
        std::cout << "Level 3: Adding 2 breakable obstacles, 1 unbreakable moving obstacle, and 3 powerups.\n";

        // Add Breakable Obstacles
        auto obstacle1 = std::make_unique<Obstacle>(
            GameState::getInstance(), "BreakableObstacle3", Obstacle::Type::Breakable,
            400.0f, 700.0f, 10.0f, 100.0f, 2, 0.0f
        );
        obstacle1->init();
        m_obstacles.push_back(std::move(obstacle1));

        auto obstacle2 = std::make_unique<Obstacle>(
            GameState::getInstance(), "BreakableObstacle4", Obstacle::Type::Breakable,
            500.0f, 250.0f, 10.0f, 100.0f, 2, 0.0f
        );
        obstacle2->init();
        m_obstacles.push_back(std::move(obstacle2));

        // Add Unbreakable Moving Obstacle
        auto obstacle3 = std::make_unique<Obstacle>(
            GameState::getInstance(), "UnbreakableObstacle1", Obstacle::Type::Unbreakable,
            350.0f, 300.0f, 10.0f, 100.0f, 0, 0.7f
        );
        obstacle3->init();
        m_obstacles.push_back(std::move(obstacle3));

        auto obstacle4 = std::make_unique<Obstacle>(
            GameState::getInstance(), "UnbreakableObstacle2", Obstacle::Type::Unbreakable,
            550.0f, 700.0f, 10.0f, 100.0f, 0, 0.7f
        );
        obstacle4->init();
        m_obstacles.push_back(std::move(obstacle4));
    }
    else if (level_number == 4)
    {
        // Level 4: Sudden Death
        std::cout << "Level 4: Sudden Death mode initialized.\n";

        m_elapsed_time = 0.0f;

        // Reset spawn counters
        m_unbreakable_obstacles_spawned_level4 = 0;
        m_breakable_obstacles_spawned_level4 = 0;
        m_powerups_spawned_level4 = 0;

        // Increase ball speed by 40%
        if (m_ball)
        {
            m_ball->setSpeed(m_ball->getSpeed() * 1.4f);
            std::cout << "Ball speed increased for Sudden Death.\n";
        }

        // Initialize spawning timers for obstacles and powerups
        // Schedule first unbreakable obstacle spawn after 2 seconds
        m_next_obstacle_spawn_time_level4 = m_elapsed_time + 2.0f;
        // Schedule first breakable obstacle spawn after 4 seconds
        m_next_breakable_obstacle_spawn_time_level4 = m_elapsed_time + 4.0f;
        // Schedule first powerup spawn after 3 seconds
        m_next_powerup_spawn_time_level4 = m_elapsed_time + 3.0f;

        std::cout << "Initialized spawning timers for Sudden Death.\n";
    }
}

/**
 * @brief Updates all objects in the level, checks for collisions, and handles timer logic.
 *
 * This method manages the game loop by updating the level timer, spawning powerups and obstacles,
 * updating players, the ball, obstacles, and powerups, handling collisions, and checking for level
 * progression or game over conditions.
 *
 * @param dt Time elapsed since the last update in seconds.
 */
void Level::update(float dt)
{
    switch (m_level_state) {
    case LevelState::MAIN_MENU:
        // Update the Main Menu
		m_menu->update();

        if (m_menu->isPlayClicked()) {
            // Start level if play is clicked
            m_level_state = LevelState::ACTIVE;
			std::cout << "Starting Level " << m_level_number << ".\n";
        }

        if (m_menu->isExitClicked())
        {
            // Handle game exit, by destroying window and terminating the program
            std::cout << "Exit pressed. Closing game.\n";
            graphics::destroyWindow();
            exit(0);
        }
        break;

    case LevelState::ACTIVE:
        // **1. Update Level Timer**
        if (m_level_number != 4) // Exclude Sudden Death from Level Timer countdown
        {
            m_level_timer -= dt / 100.0f;
            if (m_level_timer < 0.0f)
            {
                m_level_timer = 0.0f;
            }
        }

        // **Update elapsed time**
        m_elapsed_time += dt / 1000; // Assuming dt is in milliseconds; adjust if necessary

        // **2. Powerup and Obstacle Spawning Logic for Levels 2 and 3**
        if (m_level_number == 2 || m_level_number == 3) {
            if (m_total_powerups_to_spawn > 0 && m_powerups_spawned < m_total_powerups_to_spawn)
            {
                if (m_elapsed_time >= m_next_powerup_spawn_time)
                {
                    // Spawn a powerup at the next spawn position
                    if (m_powerups_spawned < m_powerup_spawn_positions.size())
                    {
                        float px = m_powerup_spawn_positions[m_powerups_spawned].first;
                        float py = m_powerup_spawn_positions[m_powerups_spawned].second;

                        // Determine powerup type based on level and spawn count
                        Powerup::Type type;
                        if (m_level_number == 2)
                        {
                            type = (m_powerups_spawned % 2 == 0) ? Powerup::Type::SPEED_UP : Powerup::Type::SLOW_DOWN;
                        }
                        else if (m_level_number == 3)
                        {
                            switch (m_powerups_spawned)
                            {
                            case 0: type = Powerup::Type::INCREASE_SIZE; break;
                            case 1: type = Powerup::Type::DECREASE_SIZE; break;
                            case 2: type = Powerup::Type::SPEED_UP; break;
                            case 3: type = Powerup::Type::SLOW_DOWN; break;
                            default: type = Powerup::Type::SPEED_UP; break;
                            }
                        }

                        // Create and initialize the Powerup object
                        auto powerup = std::make_unique<Powerup>(
                            GameState::getInstance(), "Powerup" + std::to_string(m_powerups_spawned + 1),
                            type, px, py
                        );
                        powerup->init();
                        m_powerups.push_back(std::move(powerup));

                        m_powerups_spawned++;

                        // Schedule the next powerup spawn time (e.g., add a random interval between 2-5 seconds)
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_real_distribution<float> dist_interval(2.0f, 5.0f);
                        float interval = dist_interval(gen);
                        m_next_powerup_spawn_time += interval;

                        std::cout << "Spawned Powerup" << m_powerups_spawned << " at (" << px << ", " << py << ").\n";
                    }
                    else
                    {
                        // All predefined spawn positions have been used
                        std::cout << "All powerup spawn positions have been used.\n";
                        // Optionally, reset m_powerups_spawned or handle accordingly
                    }
                }
            }
        }

        // **3. Powerup and Obstacle Spawning Logic for Level 4**
        if (m_level_number == 4) {
            // **Sudden Death: Spawn Unbreakable Obstacles**
            if (m_unbreakable_obstacles_spawned_level4 < MAX_UNBREAKABLE_OBSTACLES &&
                m_obstacles_spawned_level4 < MAX_UNBREAKABLE_OBSTACLES &&
                m_elapsed_time >= m_next_obstacle_spawn_time_level4)
            {
                // Generate random position within boundaries
                float ox = getRandomFloat(m_obstacle_spawn_min_x, m_obstacle_spawn_max_x);
                float oy = getRandomFloat(m_obstacle_spawn_min_y, m_obstacle_spawn_max_y);

                // Spawn an unbreakable obstacle
                auto obstacle = std::make_unique<Obstacle>(
                    GameState::getInstance(), "UnbreakableObstacle_SuddenDeath_" + std::to_string(m_unbreakable_obstacles_spawned_level4 + 1),
                    Obstacle::Type::Unbreakable,
                    ox, oy, 10.0f, 100.0f, 0, 0.7f
                );
                obstacle->init();
                m_obstacles.push_back(std::move(obstacle));

                m_obstacles_spawned_level4++;
                m_unbreakable_obstacles_spawned_level4++;

                // Schedule the next obstacle spawn time (e.g., add a random interval between 2-5 seconds)
                float interval = getRandomFloat(2.0f, 5.0f);
                m_next_obstacle_spawn_time_level4 = m_elapsed_time + interval;

                std::cout << "Spawned Unbreakable Obstacle " << m_unbreakable_obstacles_spawned_level4
                    << " at (" << ox << ", " << oy << ").\n";
            }

            // **Sudden Death: Spawn Breakable Obstacles**
            if (m_breakable_obstacles_spawned_level4 < MAX_BREAKABLE_OBSTACLES &&
                m_elapsed_time >= m_next_breakable_obstacle_spawn_time_level4)
            {
                // Generate random position within boundaries
                float bx = getRandomFloat(m_obstacle_spawn_min_x, m_obstacle_spawn_max_x);
                float by = getRandomFloat(m_obstacle_spawn_min_y, m_obstacle_spawn_max_y);

                // Spawn a breakable obstacle
                auto obstacle = std::make_unique<Obstacle>(
                    GameState::getInstance(), "BreakableObstacle_SuddenDeath_" + std::to_string(m_breakable_obstacles_spawned_level4 + 1),
                    Obstacle::Type::Breakable,
                    bx, by, 10.0f, 100.0f, 2, 0.0f // Assuming 2 hit points
                );
                obstacle->init();
                m_obstacles.push_back(std::move(obstacle));

                m_obstacles_spawned_level4++;
                m_breakable_obstacles_spawned_level4++;

                // Schedule the next breakable obstacle spawn time (e.g., add a random interval between 2-5 seconds)
                float interval = getRandomFloat(2.0f, 5.0f);
                m_next_breakable_obstacle_spawn_time_level4 = m_elapsed_time + interval;

                std::cout << "Spawned Breakable Obstacle " << m_breakable_obstacles_spawned_level4
                    << " at (" << bx << ", " << by << ").\n";
            }

            // **Sudden Death: Spawn Powerups**
            if (m_powerups_spawned_level4 < MAX_POWERUPS &&
                m_elapsed_time >= m_next_powerup_spawn_time_level4)
            {
                // Generate random position within boundaries
                float px = getRandomFloat(m_powerup_spawn_min_x, m_powerup_spawn_max_x);
                float py = getRandomFloat(m_powerup_spawn_min_y, m_powerup_spawn_max_y);

                // Determine powerup type based on spawn count, excluding SPEED_UP if needed
                Powerup::Type type;
                switch (m_powerups_spawned_level4 % 4) // Cycle through 4 types
                {
                case 0: type = Powerup::Type::SLOW_DOWN; break;
                case 1: type = Powerup::Type::INCREASE_SIZE; break;
                case 2: type = Powerup::Type::DECREASE_SIZE; break;
                case 3: type = Powerup::Type::SLOW_DOWN; break;
                default: type = Powerup::Type::SLOW_DOWN; break;
                }

                // **Optional: Check distance from existing powerups and obstacles before spawning**
                bool can_spawn = true;
                const float MIN_DISTANCE = 100.0f; // Minimum distance between powerups and obstacles

                for (const auto& obstacle : m_obstacles)
                {
                    if (obstacle->isActive())
                    {
                        float distance = std::sqrt(std::pow(px - obstacle->getX(), 2) + std::pow(py - obstacle->getY(), 2));
                        if (distance < MIN_DISTANCE)
                        {
                            can_spawn = false;
                            break;
                        }
                    }
                }

                for (const auto& powerup : m_powerups)
                {
                    if (powerup->isActive())
                    {
                        float distance = std::sqrt(std::pow(px - powerup->getX(), 2) + std::pow(py - powerup->getY(), 2));
                        if (distance < MIN_DISTANCE)
                        {
                            can_spawn = false;
                            break;
                        }
                    }
                }

                if (can_spawn)
                {
                    // Create and initialize the Powerup object
                    auto powerup = std::make_unique<Powerup>(
                        GameState::getInstance(), "Powerup_SuddenDeath_" + std::to_string(m_powerups_spawned_level4 + 1),
                        type, px, py
                    );
                    powerup->init();
                    m_powerups.push_back(std::move(powerup));

                    m_powerups_spawned_level4++;

                    // Schedule the next powerup spawn time (e.g., add a random interval between 2-5 seconds)
                    float interval = getRandomFloat(2.0f, 5.0f);
                    m_next_powerup_spawn_time_level4 = m_elapsed_time + interval;

                    std::cout << "Spawned Powerup " << m_powerups_spawned_level4 << " of type " << static_cast<int>(type)
                        << " at (" << px << ", " << py << ").\n";
                }
                else
                {
                    // Skip spawning if the location is too close to existing objects
                    std::cout << "Powerup spawn at (" << px << ", " << py << ") skipped due to proximity.\n";
                }
            }
        }

        

        // **4. Update Players, Ball, Obstacles, and Powerups**
        if (m_player1 && m_player1->isActive()) m_player1->update(dt);
        if (m_player2 && m_player2->isActive()) m_player2->update(dt);
        if (m_ball && m_ball->isActive()) m_ball->update(dt);

        for (auto& obstacle : m_obstacles)
        {
            if (obstacle->isActive())
                obstacle->update(dt);
        }

        for (auto& powerup : m_powerups)
        {
            if (powerup->isActive())
                powerup->update(dt);
        }

        // **5. Boundary Collision Detection and Response for Scoring**
        if (m_ball && m_ball->isActive())
        {
            float bx = m_ball->getX();
            float by = m_ball->getY();
            float halfW = m_ball->getWidth() / 2.0f;
            float halfH = m_ball->getHeight() / 2.0f;

            // **Check for collision with right boundary (Player 1 scores)**
            if (bx + halfW >= CANVAS_WIDTH)
            {
                // Increment Player1's score
                m_player1_score++;
                std::cout << "Scores - Player1: " << m_player1_score << ", Player2: " << m_player2_score << std::endl;
                m_ball->reset();

                // **Clear active powerups on the ball**
                if (m_ball)
                {
                    m_ball->clearActivePowerups();
                }

                // **Reset speed multiplier to default**
                m_speed_multiplier = 1.0f;

                std::cout << "Powerups cleared and speed multiplier reset after Player1 scoring.\n";
            }
            // **Check for collision with left boundary (Player 2 scores)**
            else if (bx - halfW <= 0.0f)
            {
                // Increment Player2's score
                m_player2_score++;
                std::cout << "Scores - Player1: " << m_player1_score << ", Player2: " << m_player2_score << std::endl;
                m_ball->reset();

                // **Clear active powerups on the ball**
                if (m_ball)
                {
                    m_ball->clearActivePowerups(); // Ensure this method exists in the Ball class
                }

                // **Reset speed multiplier to default**
                m_speed_multiplier = 1.0f;

                std::cout << "Powerups cleared and speed multiplier reset after Player2 scoring.\n";
            }

            // **6. Boundary Collision Detection and Response for Walls**

            // **Check for collision with top boundary**
            if (by + halfH >= CANVAS_HEIGHT)
            {
                m_ball->setY(CANVAS_HEIGHT - halfH);
                m_ball->setSpeed_y(-fabs(m_ball->getSpeed_y())); // Ensure speed_y is negative
                std::cout << "Ball collided with top boundary. New speed_y: " << m_ball->getSpeed_y() << std::endl;
            }

            // **Check for collision with bottom boundary**
            else if (by - halfH <= 0.0f)
            {
                m_ball->setY(halfH);
                m_ball->setSpeed_y(fabs(m_ball->getSpeed_y())); // Ensure speed_y is positive
                std::cout << "Ball collided with bottom boundary. New speed_y: " << m_ball->getSpeed_y() << std::endl;
            }
        }

        // **7. Collision Detection with Player Paddles**
        if (m_ball && m_ball->isActive())
        {
            if (!m_ball->isActivePowerup()) {
                m_speed_multiplier = 1.0f; // Reset speed multiplier if no active powerup present
            }

            // Create a Box representing the ball's current position and size
            Box ballBox(m_ball->getX(), m_ball->getY(), m_ball->getWidth(), m_ball->getHeight());

            // **Collision with Player 1 Paddle (Left Paddle)**
            if (m_player1 && m_player1->isActive())
            {
                Box paddle1Box(m_player1->getX(), m_player1->getY(), m_player1->getWidth(), m_player1->getHeight());

                if (ballBox.intersect(paddle1Box))
                {
                    // Reflect the ball's horizontal speed
                    m_ball->setSpeed_x(fabs(m_ball->getSpeed_x())); // Ensure speed_x is positive

                    // Adjust ball's position to prevent sticking
                    m_ball->setX(paddle1Box.m_pos_x + paddle1Box.m_width / 2.0f + m_ball->getWidth() / 2.0f + 1.0f);

                    std::cout << "Ball collided with Player 1 paddle. New speed_x: " << m_ball->getSpeed_x() << std::endl;

                    std::cout << "Ball speed_y adjusted by Player 1 paddle movement: New speed_y = " << m_ball->getSpeed_y() << std::endl;

                    // **Normalize the Ball's Velocity to Maintain Current Speed with Powerup**
                    float current_speed = std::sqrt(m_ball->getSpeed_x() * m_ball->getSpeed_x() +
                        m_ball->getSpeed_y() * m_ball->getSpeed_y());
                    if (current_speed > 0.0f)
                    {
                        float new_speed = m_ball->getSpeed() * m_speed_multiplier;
                        m_ball->setSpeed_x((m_ball->getSpeed_x() / current_speed) * new_speed);
                        m_ball->setSpeed_y((m_ball->getSpeed_y() / current_speed) * new_speed);
                        std::cout << "Ball velocity normalized after Player 1 paddle collision: speed_x = "
                            << m_ball->getSpeed_x() << ", speed_y = " << m_ball->getSpeed_y() << std::endl;
                    }

                    // **Optional: Play collision sound**
                    if (m_paddle_hit_sound)
                    {
                        m_paddle_hit_sound->play();
                    }
                }
            }

            // **Collision with Player 2 Paddle (Right Paddle)**
            if (m_player2 && m_player2->isActive())
            {
                Box paddle2Box(m_player2->getX(), m_player2->getY(), m_player2->getWidth(), m_player2->getHeight());

                if (ballBox.intersect(paddle2Box))
                {
                    // Reflect the ball's horizontal speed
                    m_ball->setSpeed_x(-fabs(m_ball->getSpeed_x())); // Ensure speed_x is negative

                    // Adjust ball's position to prevent sticking
                    m_ball->setX(paddle2Box.m_pos_x - paddle2Box.m_width / 2.0f - m_ball->getWidth() / 2.0f - 1.0f);

                    std::cout << "Ball collided with Player 2 paddle. New speed_x: " << m_ball->getSpeed_x() << std::endl;

                    std::cout << "Ball speed_y adjusted by Player 2 paddle movement: New speed_y = " << m_ball->getSpeed_y() << std::endl;

                    // **Normalize the Ball's Velocity to Maintain Current Speed with Powerup**
                    float current_speed = std::sqrt(m_ball->getSpeed_x() * m_ball->getSpeed_x() +
                        m_ball->getSpeed_y() * m_ball->getSpeed_y());
                    if (current_speed > 0.0f)
                    {
                        float new_speed = m_ball->getSpeed() * m_speed_multiplier;
                        m_ball->setSpeed_x((m_ball->getSpeed_x() / current_speed) * new_speed);
                        m_ball->setSpeed_y((m_ball->getSpeed_y() / current_speed) * new_speed);
                        std::cout << "Ball velocity normalized after Player 2 paddle collision: speed_x = "
                            << m_ball->getSpeed_x() << ", speed_y = " << m_ball->getSpeed_y() << std::endl;
                    }

                    // **Optional: Play collision sound**
                    if (m_paddle_hit_sound)
                    {
                        m_paddle_hit_sound->play();
                    }
                }
            }
        }

        // **8. Collision Detection with Obstacles**
        if (m_ball && m_ball->isActive())
        {
            // Create a Box representing the ball's current position and size
            Box ballBox(m_ball->getX(), m_ball->getY(), m_ball->getWidth(), m_ball->getHeight());

            for (auto& obstacle : m_obstacles)
            {
                if (obstacle->isActive())
                {
                    // Create a Box for the obstacle
                    Box obstacleBox(obstacle->getX(), obstacle->getY(), obstacle->getWidth(), obstacle->getHeight());

                    // Check for collision
                    if (ballBox.intersect(obstacleBox))
                    {
                        if (obstacle->isBreakable())
                        {
                            // Determine the side of collision
                            if (m_ball->getX() < obstacle->getX())
                            {
                                // Ball is to the left of the obstacle
                                m_ball->setSpeed_x(-fabs(m_ball->getSpeed_x())); // Ensure negative speed_x
                                m_ball->setX(obstacleBox.m_pos_x - obstacleBox.m_width / 2.0f - m_ball->getWidth() / 2.0f - 1.0f); // Prevent sticking
                                std::cout << "Ball speed updated: speed_x = " << m_ball->getSpeed_x()
                                    << ", speed_y = " << m_ball->getSpeed_y() << std::endl;
                            }
                            else
                            {
                                // Ball is to the right of the obstacle
                                m_ball->setSpeed_x(fabs(m_ball->getSpeed_x())); // Ensure positive speed_x
                                m_ball->setX(obstacleBox.m_pos_x + obstacleBox.m_width / 2.0f + m_ball->getWidth() / 2.0f + 1.0f); // Prevent sticking
                                std::cout << "Ball speed updated: speed_x = " << m_ball->getSpeed_x()
                                    << ", speed_y = " << m_ball->getSpeed_y() << std::endl;
                            }

                            std::cout << "Ball collided with breakable obstacle '" << obstacle->getName()
                                << "'. New speed_x: " << m_ball->getSpeed_x() << std::endl;

                            // **Adjust Ball's Vertical Speed Based on Obstacle's Movement**
                            m_ball->setSpeed_y(m_ball->getSpeed_y() + obstacle->getSpeed() * obstacle->getDirection());

                            std::cout << "Ball speed_y adjusted by obstacle movement: New speed_y = "
                                << m_ball->getSpeed_y() << std::endl;

                            // **Normalize the Ball's Velocity to Maintain Current Speed with Speed Multiplier**
                            float current_speed = std::sqrt(m_ball->getSpeed_x() * m_ball->getSpeed_x() +
                                m_ball->getSpeed_y() * m_ball->getSpeed_y());
                            if (current_speed > 0.0f)
                            {
                                float new_speed = m_ball->getSpeed() * m_speed_multiplier;
                                m_ball->setSpeed_x((m_ball->getSpeed_x() / current_speed) * new_speed);
                                m_ball->setSpeed_y((m_ball->getSpeed_y() / current_speed) * new_speed);
                                std::cout << "Ball velocity normalized with speed multiplier after breakable obstacle collision: speed_x = "
                                    << m_ball->getSpeed_x() << ", speed_y = " << m_ball->getSpeed_y() << std::endl;
                            }

                            // Handle the hit on the obstacle
                            obstacle->handleHit();

                            // **Handle Scoring When Breaking Breakable Obstacles**
                            if (obstacle->isBreakable() && obstacle->getHitPoints() == 0)
                            {
                                if (m_ball->getX() < obstacle->getX())
                                {
                                    // Player 1 broke the obstacle
                                    m_player1_score++;
                                    std::cout << "Player 1 broke obstacle '" << obstacle->getName()
                                        << "'. Score: " << m_player1_score << std::endl;
                                }
                                else
                                {
                                    // Player 2 broke the obstacle
                                    m_player2_score++;
                                    std::cout << "Player 2 broke obstacle '" << obstacle->getName()
                                        << "'. Score: " << m_player2_score << std::endl;
                                }
                            }

                            // Play collision sound
                            if (m_paddle_hit_sound)
                            {
                                m_paddle_hit_sound->play();
                            }

                            // Since the ball has collided with a breakable obstacle, no need to check other obstacles for this frame
                            break;
                        }
                        else
                        {
                            // Collision Logic for Unbreakable Obstacles

                            // Determine the side of collision based on vertical position
                            if (m_ball->getY() < obstacle->getY())
                            {
                                // Ball is above the obstacle
                                m_ball->setSpeed_y(-fabs(m_ball->getSpeed_y())); // Ensure speed_y is negative
                                m_ball->setY(obstacleBox.m_pos_y - obstacleBox.m_height / 2.0f - m_ball->getHeight() / 2.0f - 1.0f); // Prevent sticking
                                std::cout << "Ball collided with unbreakable obstacle '" << obstacle->getName()
                                    << "'. New speed_y: " << m_ball->getSpeed_y() << std::endl;
                            }
                            else
                            {
                                // Ball is below the obstacle
                                m_ball->setSpeed_y(fabs(m_ball->getSpeed_y())); // Ensure speed_y is positive
                                m_ball->setY(obstacleBox.m_pos_y + obstacleBox.m_height / 2.0f + m_ball->getHeight() / 2.0f + 1.0f); // Prevent sticking
                                std::cout << "Ball collided with unbreakable obstacle '" << obstacle->getName()
                                    << "'. New speed_y: " << m_ball->getSpeed_y() << std::endl;
                            }

                            // **Adjust Ball's Horizontal Speed Based on Obstacle's Movement (if necessary)**
                            // If the obstacle is moving horizontally, you might want to adjust speed_x as well
                            // Example:
                            m_ball->setSpeed_x(m_ball->getSpeed_x() + obstacle->getSpeed() * obstacle->getDirection());

                            std::cout << "Ball speed_x adjusted by obstacle movement: New speed_x = "
                                << m_ball->getSpeed_x() << std::endl;

                            // **Normalize the Ball's Velocity to Maintain Current Speed with Speed Multiplier**
                            float current_speed = std::sqrt(m_ball->getSpeed_x() * m_ball->getSpeed_x() +
                                m_ball->getSpeed_y() * m_ball->getSpeed_y());
                            if (current_speed > 0.0f)
                            {
                                float new_speed = m_ball->getSpeed() * m_speed_multiplier;
                                m_ball->setSpeed_x((m_ball->getSpeed_x() / current_speed) * new_speed);
                                m_ball->setSpeed_y((m_ball->getSpeed_y() / current_speed) * new_speed);
                                std::cout << "Ball velocity normalized with speed multiplier after unbreakable obstacle collision: speed_x = "
                                    << m_ball->getSpeed_x() << ", speed_y = " << m_ball->getSpeed_y() << std::endl;
                            }

                            // Play collision sound
                            if (m_paddle_hit_sound)
                            {
                                m_paddle_hit_sound->play();
                            }

                            // Since the ball has collided with an unbreakable obstacle, no need to check other obstacles for this frame
                            break;
                        }
                    }
                }
            }
        }

        // **9. Collision Detection with Powerups**
        if (m_ball && m_ball->isActive() && !m_ball->isRampingUp() && !m_ball->isActivePowerup())
        {
            // Create a Box representing the ball's current position and size
            Box ballBox(m_ball->getX(), m_ball->getY(), m_ball->getWidth(), m_ball->getHeight());

            // Iterate through powerups to check collision with the ball
            for (auto& powerup : m_powerups)
            {
                if (powerup && powerup->isActive())
                {
                    Box powerupBox(powerup->getX(), powerup->getY(), powerup->getWidth(), powerup->getHeight());
                    if (ballBox.intersect(powerupBox))
                    {
                        m_ball->applyPowerup(powerup->getType());

                        // **Adjust the speed multiplier based on the powerup type**
                        if (powerup->getType() == Powerup::Type::SPEED_UP)
                        {
                            m_speed_multiplier *= 1.5f; // Increase speed by 20%
                            std::cout << "Speed Up Powerup Activated. Speed Multiplier: " << m_speed_multiplier << std::endl;
                        }
                        else if (powerup->getType() == Powerup::Type::SLOW_DOWN)
                        {
                            m_speed_multiplier *= 0.6f; // Decrease speed by 20%
                            std::cout << "Slow Down Powerup Activated. Speed Multiplier: " << m_speed_multiplier << std::endl;
                        }
                        // Add additional powerup types and their effects as needed

                        // Play the powerup activation sound
                        if (m_powerup_sound)
                        {
                            m_powerup_sound->play();
                        }

                        std::cout << "Ball collided with powerup '"
                            << powerup->getName() << "'. Applying effect.\n";

                        // Mark powerup as inactive
                        powerup->setActive(false);
                    }
                }
            }
        }

        // **10. Check if it's time to progress to the next level**
        checkLevelProgression();

        // **Handle Sudden Death Winning Conditions**
        if (m_level_number == 4 && m_level_state == LevelState::ACTIVE)
        {
            if (m_player1_score >= 10)
            {
                // Player 1 wins Sudden Death
                m_winner = 1;
                m_level_state = LevelState::GAME_OVER;
                std::cout << "Player 1 wins Sudden Death with score " << m_player1_score << "!\n";
            }
            else if (m_player2_score >= 10)
            {
                // Player 2 wins Sudden Death
                m_winner = 2;
                m_level_state = LevelState::GAME_OVER;
                std::cout << "Player 2 wins Sudden Death with score " << m_player2_score << "!\n";
            }
        }
        break;

        case LevelState::PAUSE_MENU:
            // Update the pause menu
            m_menu->update();

            if (m_menu->isReadyPressed())
            {
                if (m_level_number <= 4)
                {
                    init(m_level_number, false); // Initialize the next level
                    m_level_state = LevelState::ACTIVE;
                    std::cout << "Starting Level " << m_level_number << ".\n";
                }
                else
                {
                    m_level_state = LevelState::GAME_OVER;
                    std::cout << "All levels completed. Game Over.\n";
                    // Optionally, implement Game Over logic here
                }
                m_menu->resetFlags();
            }

            // Handle exit from Pause Menu
            if (m_menu->isExitClicked())
            {
                std::cout << "Exit pressed. Closing game.\n";
                graphics::destroyWindow();
                exit(0);
            }

            break;

        case LevelState::GAME_OVER:
        {
            // **Initialize Game Over Menu if Not Already Initialized**
            if (!m_menu || m_menu->getMenuType() != MenuType::GAME_OVER_MENU)
            {
                m_menu = std::make_unique<Menu>(MenuType::GAME_OVER_MENU);
                std::cout << "Game Over Menu initialized.\n";
            }

            // **Update the Game Over Menu**
            m_menu->update();

            // **Handle Input: Press SPACE to Return to Main Menu**
            if (m_menu->isPlayClicked())
            {
                // **Reset Player Scores and Winner**
                m_player1_score = 0;
                m_player2_score = 0;
                m_winner = 0;

                // **Initialize Main Menu**
                init(1, true); // Reset to Level 1 with Main Menu
                m_level_state = LevelState::MAIN_MENU;
                std::cout << "Returning to Main Menu.\n";
            }

            // **Handle Input: Press E to Exit Game**
            if (m_menu->isExitClicked())
            {
                std::cout << "Exit pressed. Closing game.\n";
                graphics::destroyWindow(); // Ensure this correctly closes the game window
                exit(0); // Terminate the program
            }

            break;
        }
    }
    
}

/**
 * @brief Draws all level objects (players, ball, obstacles, powerups) and background.
 *
 * This method renders the background, players, ball, obstacles, powerups, and level information such as
 * scores and timers based on the current level state.
 */
void Level::draw() const
{
    switch (m_level_state) {
    case LevelState::MAIN_MENU:
    case LevelState::PAUSE_MENU:
        // Draw the Pause Menu
        if (m_menu)
        {
            m_menu->draw();
        }
        break;

    case LevelState::ACTIVE: {
        // Draw the background
        graphics::drawRect(
            CANVAS_WIDTH / 2.0f,
            CANVAS_HEIGHT / 2.0f,
            CANVAS_WIDTH,
            CANVAS_HEIGHT,
            m_bg_brush
        );

        // Draw Players
        if (m_player1 && m_player1->isActive()) m_player1->draw();
        if (m_player2 && m_player2->isActive()) m_player2->draw();

        // Draw Ball
        if (m_ball && m_ball->isActive()) m_ball->draw();

        // Draw Obstacles
        for (const auto& obstacle : m_obstacles)
        {
            if (obstacle->isActive())
                obstacle->draw();
        }

        // Draw Powerups
        for (const auto& powerup : m_powerups)
        {
            if (powerup->isActive())
                powerup->draw();
        }

        // Draw Level Information (e.g., Timer and Lives)
        graphics::Brush text_br;
        text_br.fill_opacity = 1.0f;
        text_br.outline_opacity = 0.0f;
        text_br.fill_color[0] = 1.0f; // White color
        text_br.fill_color[1] = 1.0f;
        text_br.fill_color[2] = 1.0f;

        // 1. Draw P1 Score on the Left
        std::string p1_info = "P1 Score: " + std::to_string(m_player1_score);
        graphics::drawText(
            20.0f,                               // X position (left margin)
            30.0f,                               // Y position
            20.0f,                               // Font size
            p1_info,                             // Text to display
            text_br                              // Brush settings
        );

        // 2. Draw Level and Time in the Center
        if (m_level_number == 4) {
            std::string center_info = "Level " + std::to_string(m_level_number) +
                "   |   Time left: N/A";
            graphics::drawText(
                CANVAS_WIDTH / 2.0f - 100.0f,        // X position (centered horizontally)
                30.0f,                               // Y position
                20.0f,                               // Font size
                center_info,                         // Text to display
                text_br                              // Brush settings
            );
        }
        else {
            std::string center_info = "Level " + std::to_string(m_level_number) +
                "   |   Time left: " + std::to_string(static_cast<int>(m_level_timer / 10));
            graphics::drawText(
                CANVAS_WIDTH / 2.0f - 100.0f,        // X position (centered horizontally)
                30.0f,                               // Y position
                20.0f,                               // Font size
                center_info,                         // Text to display
                text_br                              // Brush settings
            );
        }
        
        // 3. Draw P2 Score on the Right
        std::string p2_info = "P2 Score: " + std::to_string(m_player2_score);
        graphics::drawText(
            CANVAS_WIDTH - 115.0f,               // X position (right margin)
            30.0f,                               // Y position
            20.0f,                               // Font size
            p2_info,                             // Text to display
            text_br                              // Brush settings
        );
        break;
    }

    case LevelState::GAME_OVER: {
        // Draw the background
        graphics::drawRect(
            CANVAS_WIDTH / 2.0f,    // X position (centered horizontally)
            CANVAS_HEIGHT / 2.0f,   // Y position (centered vertically)
            CANVAS_WIDTH,            // Width of the rectangle
            CANVAS_HEIGHT,           // Height of the rectangle
            m_bg_brush               // Brush with texture and opacity
        );

        // Draw Game Over screen with winner info
        graphics::Brush br;
        br.outline_opacity = 0.0f;

        std::string winner_text;
        if (m_winner == 1)
        {
            br.fill_color[0] = 0.0f; // Blue color for Player1
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 1.0f;
            winner_text = "Player 1 Wins!";
        }
        else if (m_winner == 2)
        {
            br.fill_color[0] = 1.0f; // Red color for Player2
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.0f;
            winner_text = "Player 2 Wins!";
        }
        else
        {
            br.fill_color[0] = 1.0f; // Default color
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 1.0f;
            winner_text = "Game Over";
        }

        graphics::drawText(CANVAS_WIDTH / 2.0f - 140.0f, CANVAS_HEIGHT / 2.0f - 50.0f, 50.0f, winner_text, br);

        // Display final scores
        std::string final_score = "Final Scores - P1: " + std::to_string(m_player1_score) +
            " | P2: " + std::to_string(m_player2_score);
        graphics::Brush score_br;
        score_br.fill_color[0] = 1.0f;
        score_br.fill_color[1] = 1.0f;
        score_br.fill_color[2] = 1.0f;
        score_br.outline_opacity = 0.0f;
        graphics::drawText(CANVAS_WIDTH / 2.0f - 148.0f, CANVAS_HEIGHT / 2.0f + 10.0f, 30.0f, final_score, score_br);

        // **Add Replay Instruction Below Final Scores**
        std::string replay_info = "Press R to Replay the Game";
        graphics::Brush replay_br;
        replay_br.fill_color[0] = 1.0f; // White color
        replay_br.fill_color[1] = 1.0f;
        replay_br.fill_color[2] = 1.0f;
        replay_br.outline_opacity = 0.0f;
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 168.0f, // X position (centered horizontally)
            CANVAS_HEIGHT / 2.0f + 50.0f, // Y position (below final scores)
            30.0f,                        // Font size
            replay_info,                  // Text to display
            replay_br                     // Brush settings
        );

        break;
    }
        
    }
    
}

/**
 * @brief Checks if it's time to advance to the next level based on the timer.
 *
 * Monitors the level timer and triggers progression to the next level when the time expires.
 */
void Level::checkLevelProgression()
{
    // If time is up, proceed to next level
    if (m_level_timer <= 0.0f)
    {
        nextLevel();
    }
}

/**
 * @brief Moves to the next level or transitions to Sudden Death based on current progress.
 *
 * Advances the game to the subsequent level if applicable. After completing level 3,
 * checks player scores to determine if Sudden Death should be initiated or if a player has won.
 */
void Level::nextLevel()
{
    if (m_level_number < 3)
    {
        // Advance to the next level and initialize it with the menu
        m_level_number++;
        init(m_level_number, true); // Show Pause Menu for Level 2 and 3
        std::cout << "Advancing to Level " << m_level_number << ".\n";
    }
    else if (m_level_number == 3)
    {
        // After completing level 3, evaluate scores to determine the winner or start Sudden Death
        if (m_player1_score > m_player2_score)
        {
            // Player 1 wins the game
            m_winner = 1;
            m_level_state = LevelState::GAME_OVER;
            std::cout << "Player 1 wins with score " << m_player1_score
                << " to " << m_player2_score << ".\n";
        }
        else if (m_player2_score > m_player1_score)
        {
            // Player 2 wins the game
            m_winner = 2;
            m_level_state = LevelState::GAME_OVER;
            std::cout << "Player 2 wins with score " << m_player2_score
                << " to " << m_player1_score << ".\n";
        }
        else
        {
            // Scores are equal, start sudden death
            m_level_number = 4;
            init(m_level_number, true); // Show Pause Menu or Ready Menu for Sudden Death
            std::cout << "Scores tied. Advancing to Level 4: Sudden Death.\n";
        }
    }
    else
    {
        // **After Level 4: Reset the game by displaying the Main Menu**

        // **Reset the level number to 1 to prepare for a new game**
        m_level_number = 1;

        // **Initialize Level 1 with the Main Menu**
        init(m_level_number, true); // Show Main Menu for Level 1

        std::cout << "Sudden Death completed. Returning to Main Menu.\n";
    }
}

