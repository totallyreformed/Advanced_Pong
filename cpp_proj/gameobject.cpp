#include "GameObject.h"
#include "GameState.h"

// Initialize static member
int GameObject::m_next_id = 1;

/**
 * @brief Constructor for GameObject.
 */
GameObject::GameObject(GameState* gs, const std::string& name)
    : m_state(gs),
    m_name(name),
    m_id(m_next_id++),
    m_active(true),
    x(0.0f),
    y(0.0f),
    m_width(0.0f),
    m_height(0.0f)
{
    // ...
}
