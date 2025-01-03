#pragma once
#include <string>

// Forward declaration to avoid circular dependencies
class GameState;

/**
 * @class GameObject
 * @brief Base class for all game objects (players, ball, obstacles, powerups, etc.).
 */
class GameObject
{
protected:
    static int m_next_id;        ///< Static member for automatic ID incrementation.
    GameState* m_state;          ///< Pointer to the GameState managing the object.
    std::string m_name;          ///< Name of the object.
    int m_id;                    ///< Unique ID of the object.
    bool m_active;               ///< Active state of the object.

    float x;                     ///< Horizontal position of the object.
    float y;                     ///< Vertical position of the object.
    float m_width;
    float m_height;

public:
    /**
     * @brief Constructor for GameObject.
     * @param gs Pointer to the GameState.
     * @param name Name of the object.
     */
    GameObject(GameState* gs, const std::string& name = "");

    /**
     * @brief Virtual destructor.
     */
    virtual ~GameObject() {}

    /**
     * @brief Updates the state of the object.
     * @param dt Time elapsed since the last update in seconds.
     */
    virtual void update(float dt) {}

    /**
     * @brief Initializes the object.
     */
    virtual void init() {}

    /**
     * @brief Draws the object on the screen.
     */
    virtual void draw() {}

    /**
     * @brief Checks if the object is active.
     * @return True if active, otherwise false.
     */
    bool isActive() const { return m_active; }

    /**
     * @brief Sets the active state of the object.
     * @param a The new active state.
     */
    void setActive(bool a) { m_active = a; }

    /**
     * @brief Retrieves the name of the object.
     * @return The name of the object.
     */
    std::string getName() const { return m_name; }

    /**
     * @brief Retrieves the unique ID of the object.
     * @return The ID of the object.
     */
    int getID() const { return m_id; }

    /**
     * @brief Retrieves the horizontal position.
     */
    float getX() const { return x; }

    /**
     * @brief Retrieves the vertical position.
     */
    float getY() const { return y; }

    /**
     * @brief Sets the horizontal position.
     */
    void setX(float newX) { x = newX; }

    /**
     * @brief Sets the vertical position.
     */
    void setY(float newY) { y = newY; }

    /**
     * @brief Retrieves the width of the object.
     */
    float getWidth() const { return m_width; }

    /**
     * @brief Retrieves the height of the object.
     */
    float getHeight() const { return m_height; }

    /**
     * @brief Sets the width of the object.
     */
    void setWidth(float newWidth) { m_width = newWidth; }

    /**
     * @brief Sets the height of the object.
     */
    void setHeight(float newHeight) { m_height = newHeight; }
};
