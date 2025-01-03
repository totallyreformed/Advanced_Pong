// Menu.cpp
#include "Menu.h"
#include "sgg/graphics.h"
#include <iostream>
#include "config.h"

/**
 * @brief Constructor initializes the menu type and flags.
 * @param type The type of menu to initialize.
 */
Menu::Menu(MenuType type)
    : m_type(type), m_play_clicked(false), m_exit_clicked(false), m_ready_pressed(false)
{
}

/**
 * @brief Sets the menu type and resets flags.
 * @param type The new type of the menu.
 */
void Menu::setMenuType(MenuType type)
{
    m_type = type;
    resetFlags();
}

/**
 * @brief Resets the menu state flags.
 */
void Menu::resetFlags()
{
    m_play_clicked = false;
    m_exit_clicked = false;
    m_ready_pressed = false;
}

/**
 * @brief Draws the menu based on its type.
 */
void Menu::draw() const
{
    graphics::Brush br;
    br.fill_color[0] = 0.0f; // Black background
    br.fill_color[1] = 0.0f;
    br.fill_color[2] = 0.0f;
    br.fill_opacity = 1.0f;
    br.outline_opacity = 0.0f;

    // Draw full-screen rectangle as background
    graphics::drawRect(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    br.fill_color[0] = 1.0f; // White text
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;
    br.fill_opacity = 1.0f;
    br.outline_opacity = 0.0f;

    if (m_type == MenuType::MAIN_MENU)
    {
        // Draw Play Button Text
        graphics::drawText(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f - 50.0f, 30.0f, "Press SPACE to Play", br);

        // Draw Exit Button Text
        graphics::drawText(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f + 50.0f, 30.0f, "Press E to Exit", br);
    }
    else if (m_type == MenuType::PAUSE_MENU)
    {
        // Draw Ready? Sign
        graphics::drawText(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f, 50.0f, "Ready?", br);
        graphics::drawText(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f + 60.0f, 20.0f, "Press SPACE to Continue", br);
    }
}

/**
 * @brief Handles input events for the menu.
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
}

/**
 * @brief Checks if the Play button was pressed (Main Menu only).
 * @return True if Play was pressed, otherwise False.
 */
bool Menu::isPlayClicked() const
{
    return m_play_clicked;
}

/**
 * @brief Checks if the Exit button was pressed (Main Menu only).
 * @return True if Exit was pressed, otherwise False.
 */
bool Menu::isExitClicked() const
{
    return m_exit_clicked;
}

/**
 * @brief Checks if the Ready button was pressed (Pause Menu only).
 * @return True if Ready (spacebar) was pressed, otherwise False.
 */
bool Menu::isReadyPressed() const
{
    return m_ready_pressed;
}
