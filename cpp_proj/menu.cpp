// Menu.cpp
#include "Menu.h"
#include "sgg/graphics.h"
#include <iostream>
#include "config.h"
#include "gamestate.h"

/**
 * @text_brief Constructor initializes the menu type and flags.
 * @param type The type of menu to initialize.
 */
Menu::Menu(MenuType type)
    : m_type(type), m_play_clicked(false), m_exit_clicked(false), m_ready_pressed(false)
{
    // **Initialize the Background Brush**
    m_bg_brush.texture = GameState::getInstance()->getFullAssetPath("background.png");
    m_bg_brush.fill_opacity = 0.17f;     // Semi-transparent fill
    m_bg_brush.outline_opacity = 0.0f;  // No outline
}

/**
 * @text_brief Sets the menu type and resets flags.
 * @param type The new type of the menu.
 */
void Menu::setMenuType(MenuType type)
{
    m_type = type;
    resetFlags();
}

/**
 * @text_brief Resets the menu state flags.
 */
void Menu::resetFlags()
{
    m_play_clicked = false;
    m_exit_clicked = false;
    m_ready_pressed = false;
}

/**
 * @text_brief Draws the menu based on its type.
 */
void Menu::draw() const
{
    // **Draw the Background Rectangle Covering the Entire Canvas**
    graphics::drawRect(
        CANVAS_WIDTH / 2.0f,    // Center X
        CANVAS_HEIGHT / 2.0f,   // Center Y
        CANVAS_WIDTH,            // Width
        CANVAS_HEIGHT,           // Height
        m_bg_brush               // Brush with texture and opacity
    );

    // 2) Set the font and define a brush for text

    // Create a Separate Brush for the Title
    graphics::resetPose();
    graphics::Brush title_br;
    title_br.fill_color[0] = 0.0f; // Red component (0.0f - 1.0f)
    title_br.fill_color[1] = 0.5f; // Green component (0.0f - 1.0f)
    title_br.fill_color[2] = 1.0f; // Blue component (0.0f - 1.0f)
    title_br.fill_opacity = 1.0f;  // Fully opaque
    title_br.outline_opacity = 0.0f; // No outline


    graphics::setFont(GameState::getInstance()->getFullAssetPath("ARIAL.ttf"));
    graphics::resetPose();
    graphics::Brush text_br;
    text_br.fill_color[0] = 1.0f;   // White text
    text_br.fill_color[1] = 1.0f;
    text_br.fill_color[2] = 1.0f;
    text_br.outline_opacity = 0.0f;
    text_br.fill_opacity = 1.0f;

    // 3) Draw text according to the current menu type
    if (m_type == MenuType::MAIN_MENU)
    {
        // **a. Draw Title "Advanced Pong"**
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 195.0f,    // Center the title horizontally
            CANVAS_HEIGHT / 2.0f - 100.0f,   // Place title higher
            60.0f,                           // Font size for title
            "Advanced Pong",
            title_br
        );

        // **b. Draw Play Button Text**
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 130.0f,    // Center horizontally
            CANVAS_HEIGHT / 2.0f - 20.0f,    // Adjust position below the title
            30.0f,
            "Press SPACE to Play",
            text_br
        );

        // **c. Draw Exit Button Text**
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 130.0f,    // Center horizontally
            CANVAS_HEIGHT / 2.0f + 40.0f,    // Adjust position below the play text
            30.0f,
            "Press E to Exit Game",
            text_br
        );
    }

    else if (m_type == MenuType::PAUSE_MENU)
    {
        // Centered "Ready?"
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 75.0f,    // Adjusted to center horizontally
            CANVAS_HEIGHT / 2.0f - 20.0f,   // Centered vertically
            50.0f,                          // Font size for "Ready?"
            "Ready?",
            text_br
        );

        // Centered "Press SPACE to Continue"
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 175.0f,   // Adjusted to center horizontally
            CANVAS_HEIGHT / 2.0f + 40.0f,   // Positioned below "Ready?"
            20.0f,                          // Font size for instructions
            "Press SPACE to Continue to the next Level",
            text_br
        );
    }
    else if (m_type == MenuType::GAME_OVER_MENU)
    {
        // **a. Draw "Game Over" Title**
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 100.0f,    // Centered horizontally
            CANVAS_HEIGHT / 2.0f - 100.0f,   // Positioned higher vertically
            50.0f,                            // Font size
            "Game Over",
            text_br
        );

        // **b. Draw "Press R to return to the Main Menu" Instruction**
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 150.0f,    // Centered horizontally
            CANVAS_HEIGHT / 2.0f,             // Centered vertically
            30.0f,                            // Font size
            "Press R to Play Again",
            text_br
        );

        // **c. Draw "Press E to Exit Game" Instruction**
        graphics::drawText(
            CANVAS_WIDTH / 2.0f - 130.0f,    // Centered horizontally
            CANVAS_HEIGHT / 2.0f + 40.0f,    // Positioned below the first instruction
            30.0f,                            // Font size
            "Press E to Exit Game",
            text_br
        );
    }
}


/**
 * @text_brief Handles input events for the menu.
 */
void Menu::update()
{
    // Handle Main Menu Inputs
    if (m_type == MenuType::MAIN_MENU)
    {
        bool current_spacebar = graphics::getKeyState(graphics::SCANCODE_SPACE);
        bool current_e = graphics::getKeyState(graphics::SCANCODE_E);

        // Play Button: Detect key down event
        if (current_spacebar && !m_previous_spacebar_state)
        {
            m_play_clicked = true;
            std::cout << "Play button pressed (Spacebar).\n";
        }

        // Exit Button: Detect key down event
        if (current_e && !m_previous_e_state)
        {
            m_exit_clicked = true;
            std::cout << "Exit button pressed ('E').\n";
        }

        // Update previous states
        m_previous_spacebar_state = current_spacebar;
        m_previous_e_state = current_e;
    }
    // Handle Pause Menu Inputs
    else if (m_type == MenuType::PAUSE_MENU)
    {
        bool current_spacebar = graphics::getKeyState(graphics::SCANCODE_SPACE);

        // Continue Button: Detect key down event
        if (current_spacebar && !m_previous_spacebar_state)
        {
            m_ready_pressed = true;
            std::cout << "Continue pressed (Spacebar).\n";
        }

        // Update previous state
        m_previous_spacebar_state = current_spacebar;
    }   
    else if (m_type == MenuType::GAME_OVER_MENU)
    {
        bool current_r = graphics::getKeyState(graphics::SCANCODE_R);
        bool current_e = graphics::getKeyState(graphics::SCANCODE_E);

        // **a. Detect R Press for Returning to Main Menu**
        if (current_r && !m_previous_spacebar_state)
        {
            m_play_clicked = true; // Reuse the play_clicked flag for Main Menu return
            std::cout << "Main Menu pressed (R).\n";
        }

        // **b. Detect E Press for Exiting the Game**
        if (current_e && !m_previous_e_state)
        {
            m_exit_clicked = true;
            std::cout << "Exit button pressed ('E').\n";
        }

        // **c. Update Previous Key States**
        m_previous_r_state = current_r;
        m_previous_e_state = current_e;
    }
}

/**
 * @text_brief Checks if the Play button was pressed (Main Menu only).
 * @return True if Play was pressed, otherwise False.
 */
bool Menu::isPlayClicked() const
{
    return m_play_clicked;
}

/**
 * @text_brief Checks if the Exit button was pressed (Main Menu only).
 * @return True if Exit was pressed, otherwise False.
 */
bool Menu::isExitClicked() const
{
    return m_exit_clicked;
}

/**
 * @text_brief Checks if the Ready button was pressed (Pause Menu only).
 * @return True if Ready (spacebar) was pressed, otherwise False.
 */
bool Menu::isReadyPressed() const
{
    return m_ready_pressed;
}
