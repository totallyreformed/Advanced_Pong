// Menu.h
#pragma once

#include "sgg/graphics.h"
#include <string>

/**
 * @enum MenuType
 * @brief Defines the type of menu (Main Menu or Pause Menu).
 */
enum class MenuType
{
    MAIN_MENU,
    PAUSE_MENU,
    GAME_OVER_MENU
};

/**
 * @class Menu
 * @brief Handles drawing and input for different types of menus.
 */
class Menu
{
private:
    MenuType m_type;               ///< Current type of the menu.
    bool m_play_clicked;           ///< Flag to indicate if Play button was pressed.
    bool m_exit_clicked;           ///< Flag to indicate if Exit button was pressed.
    bool m_ready_pressed;          ///< Flag to indicate if Ready (spacebar) was pressed.

    // Debounce flags to prevent multiple triggers from a single key press
    bool m_previous_spacebar_state = false;
    bool m_previous_e_state = false;
    bool m_previous_r_state = false;

    // Declare the Background Brush
    graphics::Brush m_bg_brush;

public:
    /**
     * @brief Constructor initializes the menu type.
     * @param type The type of menu to initialize.
     */
    Menu(MenuType type = MenuType::MAIN_MENU);

    /**
     * @brief Draws the menu based on its type.
     */
    void draw() const;

    /**
     * @brief Handles input events for the menu.
     */
    void update();

    /**
     * @brief Sets the menu type.
     * @param type The new type of the menu.
     */
    void setMenuType(MenuType type);

	/**
    * @brief Retrieves the current menu type.
	* @return The current menu type.
	*/
	MenuType getMenuType() const { return m_type; }

    /**
     * @brief Checks if the Play button was pressed (Main Menu only).
     * @return True if Play was pressed, otherwise False.
     */
    bool isPlayClicked() const;

    /**
     * @brief Checks if the Exit button was pressed (Main Menu only).
     * @return True if Exit was pressed, otherwise False.
     */
    bool isExitClicked() const;

    /**
     * @brief Checks if the Ready button was pressed (Pause Menu only).
     * @return True if Ready (spacebar) was pressed, otherwise False.
     */
    bool isReadyPressed() const;

    /**
     * @brief Resets the menu state flags.
     */
    void resetFlags();
};
